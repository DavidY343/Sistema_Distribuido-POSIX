#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "claves.h"
#include "structs_handler.h"

pthread_mutex_t mutex_mensaje;
pthread_cond_t cond_mensaje;
int not_finished = true;
mqd_t  q_server;



void treat_request(void *mess)
{
	struct request message;	/* mensaje local */
	struct response res;
	mqd_t q_client;		/* cola del cliente */

	pthread_mutex_lock(&mutex_mensaje);
	message = (*(struct request *) mess);
	switch (message.op)
	{
		case 0:
			res.error = init();
			break;
		case 1:
			res.error = set_value(message.key, message.v1, message.N, message.v2);
			break;
		case 2:
			res.error = get_value(message.key, message.v1, &message.N, message.v2);
			break;
		case 3:
			res.error = modify_value(message.key, message.v1, message.N, message.v2);
			break;
		case 4:
			res.error = delete_key(message.key);
			break;
		case 5:
			res.error = exist(message.key);
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
		printf("Voy a enviar\n");
		if (mq_send(q_client, ( char *) &res, sizeof(struct request), 0) < 0)
		{
			perror("mq_send");
			mq_close(q_server);
			mq_unlink("/SERVER");
			mq_close(q_client);
		}
		printf("Enviado\n");
	}
	pthread_exit(0);
}

void print_message(struct request message) {
    printf("Operation: %d\n", message.op);
    printf("Node: key=%d, v1=%s, N=%d\n", message.key, message.v1, message.N);
    printf("Queue: %s\n", message.queue);
	printf("Contenido de v2:\n");
    for (int i = 0; i < message.N; i++) {
        printf("%f ", message.v2[i]);
    }
    printf("\n");
}

int main(void)
	{
	struct request message;
	struct mq_attr attr;
	pthread_attr_t t_attr;
	pthread_t thid;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(struct request);
	q_server = mq_open("/SERVER", O_CREAT | O_RDONLY, 0777, &attr);
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
		if (mq_receive(q_server, (char *) &message, sizeof(struct request), NULL) < 0 )
		{
			perror("mq_receive");
			return -1;
		}
		print_message(message);
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