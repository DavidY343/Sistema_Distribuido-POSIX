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

void treat_request(void *mess) {
	struct request mensaje;	/* mensaje local */
	mqd_t q_client;		/* cola del cliente */
	int error;

	/* el thread copia el mensaje a un mensaje local */
	pthread_mutex_lock(&mutex_mensaje);

	mensaje = (*(struct request *) mess);

	/* ya se puede despertar al servidor*/
	not_finished = false;


	switch (mensaje.operation) {
		case 0:
			error = init();
			break;
		case 1:
			error = set_value(mensaje.node.key, mensaje.node.v1, mensaje.node.N, mensaje.node.v2);
			break;
		case 2:
			error = get_value(mensaje.node.key, mensaje.node.v1, mensaje.node.N, mensaje.node.v2);
			break;
		case 3:
			error = modify_value(mensaje.node.key, mensaje.node.v1, mensaje.node.N, mensaje.node.v2);
			break;
		case 4:
			error = delete_key(mensaje.node.key);
			break;
		case 5:
			error = exist(mensaje.node.key);
			break;
	}
	
	pthread_cond_signal(&cond_mensaje);
	pthread_mutex_unlock(&mutex_mensaje);

		q_client = mq_open(mensaje.queue, O_WRONLY);
	if (q_client == -1){
		perror("No se puede abrir la cola del cliente");
		mq_close(q_server);
		mq_unlink("/SERVER");
	}
	else {
		if (mq_send(q_client, (const char *) &error, sizeof(int), 0) <0) {
			perror("mq_send");
			mq_close(q_server);
			mq_unlink("/SERVER");
			mq_close(q_client);
		}
	}
	pthread_exit(0);
}

int main(void) {
    struct request mess;
    struct mq_attr attr;
    pthread_attr_t t_attr;
   	pthread_t thid;

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct request);

    q_server = mq_open("/SERVER", O_CREAT|O_RDONLY, 0700, &attr);
    if (q_server == -1) {
        perror("mq_open");
        return -1;
    }
    pthread_mutex_init(&mutex_mensaje, NULL);
    pthread_cond_init(&cond_mensaje, NULL);
    pthread_attr_init(&t_attr);

    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

    while(1) {
        if (mq_receive(q_server, (char *) &mess, sizeof(mess), 0) < 0 ){
            perror("mq_receive");
            return -1;
        }

        if (pthread_create(&thid, &t_attr, (void *)treat_request, (void *)&mess)== 0) {
			// se espera a que el thread copie el mensaje 
			pthread_mutex_lock(&mutex_mensaje);
			while (not_finished)
				pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
			not_finished = true;
			pthread_mutex_unlock(&mutex_mensaje);
	 	}   
    }

}