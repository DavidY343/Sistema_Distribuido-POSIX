
#include "proxy.h"

mqd_t q_client; 
mqd_t q_server; 
struct request message;	
struct response res;
struct mq_attr attr;
char queue[MAX_VALUE_LENGTH];


int init_queue()
{
	attr.mq_maxmsg = 1;     
	attr.mq_msgsize = sizeof(struct request);

	sprintf(queue, "/Queue-%d", getpid());
	q_client = mq_open(queue, O_CREAT|O_RDONLY, 0666, &attr);
	if (q_client == -1)
	{
		perror("mq_open 1");
		return (-1);
	}
	q_server = mq_open("/SERVER", O_CREAT | O_WRONLY, 0777);
	if (q_server == -1)
	{
		perror("mq_open");
		return (-1);
	}
	return (0);
}

int communication(struct request message, struct response *res)
{
	// printf("Tamano de struct: %ld, Tamano de mensaje: %ld\n", sizeof(struct request), sizeof(message));
	// printf("Tamaño por separado: %ld \n", sizeof(message.key) + sizeof(message.N) + sizeof(message.op) + sizeof(message.queue) + sizeof(message.v1) + sizeof(message.v2));
	if (mq_send(q_server, (const char *)&message, sizeof(struct request), 0) < 0)
	{
		perror("mq_send");
		mq_close(q_server);
        mq_close(q_client);
        mq_unlink(queue);
		return (-1);
	}
	if (mq_receive(q_client, (char *)res, sizeof(struct response), 0) < 0)
	{
		perror("mq_receive");
		mq_close(q_server);
        mq_close(q_client);
        mq_unlink(queue);
		return (-1);
	}
	return (0);
}
static int check_value(char *value1, int N_value2, double *V_value2)
{
	if (strlen(value1) >= 256)
		return (-1);
	if (N_value2 < 1 || N_value2 > 32)
		return (-1);
	return (0);
}
int init_proxy()
{
	struct request message;	
	struct response res;
	if (init_queue() == -1)
	{
		printf("Error initialiting queue\n");
		return (-1);
	}
	message.op = 0;
	strcpy(message.queue, queue);
	if (communication(message, &res) == -1)
	{
		printf("Error in coms with server\n");
		return (-1);
	}
	if (res.error != 0)
	{
		printf("Error in init\n");
		return (-1);
	}		
	else
	{
		printf("Linked list emptied\n");
		return (0);
	}
}

int set_value_proxy(int key, char *value1, int N_value2, double *V_value2)
{
	struct request message;	
	struct response res;
	if (init_queue() == -1)
	{
		printf("Error initialiting queue\n");
		return (-1);
	}
	if (check_value(value1, N_value2, V_value2) == -1)
	{
		printf("Not the right values\n");
		return (-1);
	}
	message.op = 1;
	strcpy(message.queue, queue);
	message.key = key;
	message.N = N_value2;
	strcpy(message.v1, value1);
	for (int i = 0; i < N_value2; i++)
    	message.v2[i] = V_value2[i];
	if (communication(message, &res) == -1)
	{
		printf("Error in coms with server\n");
		return (-1);
	}
	if (res.error != 0)
	{
		printf("Error in set_value\n");
		return (-1);
	}
	else
	{
		printf("Value set\n");
		return (0);
	}
}

int get_value_proxy(int key, char *value1, int *N_value2, double *V_value2)
{
	struct request message;	
	struct response res;
	if (init_queue() == -1)
	{
		printf("Error initialiting queue\n");
		return (-1);
	}
	message.op = 2;
	strcpy(message.queue, queue);
	message.key = key;
	message.N = *N_value2;
	strcpy(message.v1, value1);
	memcpy(message.v2, V_value2, *N_value2 * sizeof(double));
	if (communication(message, &res) == -1)
	{
		printf("Error in coms with server\n");
		return (-1);
	}
	if (res.error != 0)
	{
		printf("Error in get_value\n");
		return (-1);
	}
	else
	{
		printf("Your values: v1: %s, N2: %d\n", res.v1, res.N);
		strcpy(value1, res.v1);
		*N_value2 = res.N;
		printf("Contenido de v2:\n");
    	for(int i = 0; i < res.N; i++)
		{
        	printf("%f ", res.v2[i]);
			V_value2[i] = res.v2[i];
		}
		printf("\n");
		return (0);
	}
}

int modify_value_proxy(int key, char *value1, int N_value2, double *V_value2)
{
	struct request message;	
	struct response res;
	if (init_queue() == -1)
	{
		printf("Error initialiting queue\n");
		return (-1);
	}
	if (check_value(value1, N_value2, V_value2) == -1)
	{
		printf("Not the right values\n");
		return (-1);
	}
	message.op = 3;
	strcpy(message.queue, queue);
	message.key = key;
	message.N = N_value2;
	strcpy(message.v1, value1);
	memcpy(message.v2, V_value2, N_value2 * sizeof(double));
	if (communication(message, &res) == -1)
	{
		printf("Error in coms with server\n");
		return (-1);
	}
	if (res.error != 0)
	{
		printf("Error in modify_value\n");
		return (-1);
	}
	else
	{
		printf("Modify succesful\n");
		return (0);
	}
}

int delete_key_proxy(int key)
{
	struct request message;	
	struct response res;
	if (init_queue() == -1)
	{
		printf("Error initialiting queue\n");
		return (-1);
	}
	message.op = 4;
	strcpy(message.queue, queue);
	message.key = key;
	if (communication(message, &res) == -1)
	{
		printf("Error in coms with server\n");
		return (-1);
	}
	if (res.error != 0)
	{
		printf("Error in delete\n");
		return (-1);
	}
	else
	{
		printf("deleted sucessful\n");
		return (0);
	}
}

int exist_proxy(int key)
{
	struct request message;	
	struct response res;
	if (init_queue() == -1)
	{
		printf("Error initialiting queue\n");
		return (-1);
	}
	message.op = 5;
	strcpy(message.queue, queue);
	message.key = key;
	if (communication(message, &res) == -1)
	{
		printf("Error in coms with server\n");
		return (-1);
	}
	if (res.error != 0)
	{
		printf("Does not exist\n");
		return (-1);
	}
	else
	{
		printf("Exists\n");
		return (0);
	}	
}