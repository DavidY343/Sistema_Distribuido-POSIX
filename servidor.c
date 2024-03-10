#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "claves.h"
#include "structs_handler.h"

pthread_mutex_t mutex_mensaje;
pthread_cond_t cond_mensaje;
int not_finished = true;
mqd_t  q_server;

void treat_request(void *mess)
{
	struct request message;	/* mensaje local */
	mqd_t q_client;		/* cola del cliente */
	int error;
	
	pthread_mutex_lock(&mutex_mensaje);
	message = (*(struct request *) mess);
	switch (message.op)
	{
		case 0:
			error = init();
			break;
		case 1:
			error = set_value(message.node.key, message.node.v1, message.node.N, message.node.v2);
			break;
		case 2:
			error = get_value(message.node.key, message.node.v1, &message.node.N, message.node.v2);
			break;
		case 3:
			error = modify_value(message.node.key, message.node.v1, message.node.N, message.node.v2);
			break;
		case 4:
			error = delete_key(message.node.key);
			break;
		case 5:
			error = exist(message.node.key);
			break;
	}
	not_finished = false;
	pthread_cond_signal(&cond_mensaje);
	pthread_mutex_unlock(&mutex_mensaje);
	q_client = mq_open(message.queue, O_WRONLY);
	if (q_client == -1)
	{
		perror("No se puede abrir la cola del cliente");
		mq_close(q_server);
		mq_unlink("/SERVER");
	}
	else
	{
		if (mq_send(q_client, (const char *) &error, sizeof(int), 0) <0)
		{
			perror("mq_send");
			mq_close(q_server);
			mq_unlink("/SERVER");
			mq_close(q_client);
		}
	}
	pthread_exit(0);
}


int main(void)
	{
	struct request message;
	struct mq_attr attr;
	pthread_attr_t t_attr;
	pthread_t thid;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(struct request);

	q_server = mq_open("/SERVER", O_CREAT|O_RDONLY, 0700, &attr);
	if (q_server == -1)
	{
		perror("mq_open");
		return (-1);
	}
	pthread_mutex_init(&mutex_mensaje, NULL);
	pthread_cond_init(&cond_mensaje, NULL);
	pthread_attr_init(&t_attr);

	pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

	while(1)
	{
		if (mq_receive(q_server, (char *) &message, sizeof(message), 0) < 0 )
		{
			perror("mq_receive");
			return -1;
		}

		if (pthread_create(&thid, &t_attr, (void *)treat_request, (void *)&message)== 0)
		{
			pthread_mutex_lock(&mutex_mensaje);
			while (not_finished)
				pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
			not_finished = true;
			pthread_mutex_unlock(&mutex_mensaje);
		}   
	}
}