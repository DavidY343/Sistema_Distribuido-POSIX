
#include "proxy.h"

mqd_t q_client; 
mqd_t q_server; 
struct request message;
int response;
struct mq_attr attr;
char queue[MAX_VALUE_LENGTH];


int init_queue()
{
	attr.mq_maxmsg = 1;     
	attr.mq_msgsize = sizeof(struct request);

	sprintf(queue, "/Queue-%d", getpid());
	q_client = mq_open(queue, O_CREAT|O_RDONLY, 0700, &attr);
	if (q_client == -1)
	{
		perror("mq_open 1");
		return (-1);
	}
	q_server = mq_open("/SERVER", O_WRONLY, 0700, &attr);
	if (q_server == -1)
	{
		perror("mq_open 2");
		return (-1);
	}
	return (0);
}

int communication()
{
	if (mq_send(q_server, (const char *)&message, sizeof(struct request), 0) < 0)
	{
		perror("mq_send");
		return (-1);
	}
	if (mq_receive(q_client, (char *)&response, sizeof(int), 0) < 0)
	{
		perror("mq_receive");
		return (-1);
	}
	return (0);
}

int init_proxy()
{
	if (init_queue() == -1)
		return (-1);
	message.op = 0;
	strcpy(message.queue, queue);
	if (communication() == -1)
		return (-1);
	if (response != 0)
		printf("Error in init\n");
	else
		printf("Linked list emptied\n");
	return (0);
}

int set_value_proxy(int key, char *value1, int N_value2, double *V_value2)
{
	if (init_queue() == -1)
		return (-1);
	message.op = 1;
	strcpy(message.queue, queue);
	message.node.key = key;
	message.node.N = N_value2;
	strcpy(message.node.v1, value1);
	memcpy(message.node.v2, V_value2, N_value2 * sizeof(double));
	if (communication() == -1)
		return (-1);
	if (response != 0)
		printf("Error in set_value\n");
	else
		printf("Set value\n");
	return (0);
}

int get_value_proxy(int key, char *value1, int *N_value2, double *V_value2)
{
	if (init_queue() == -1)
		return (-1);
	message.op = 2;
	strcpy(message.queue, queue);
	message.node.key = key;
	message.node.N = *N_value2;
	strcpy(message.node.v1, value1);
	memcpy(message.node.v2, V_value2, *N_value2 * sizeof(double));
	if (communication() == -1)
		return (-1);
	if (response != 0)
		printf("Error in get_value\n");
	else
		printf("Your values: v1: %s, N2: %d\n", message.node.v1, message.node.N);
	return (0);
}

int modify_value_proxy(int key, char *value1, int N_value2, double *V_value2)
{
	if (init_queue() == -1)
		return (-1);
	message.op = 3;
	strcpy(message.queue, queue);
	message.node.key = key;
	message.node.N = N_value2;
	strcpy(message.node.v1, value1);
	memcpy(message.node.v2, V_value2, N_value2 * sizeof(double));
	if (communication() == -1)
		return (-1);
	if (response != 0)
		printf("Error in modify_value\n");
	else
		printf("Modify succesful");
	return (0);
}

int delete_key_proxy(int key)
{
	if (init_queue() == -1)
		return (-1);
	message.op = 4;
	strcpy(message.queue, queue);
	message.node.key = key;
	if (communication() == -1)
		return (-1);
	if (response != 0)
		printf("Error in delete\n");
	else
		printf("deleted sucessful\n");
	return (0);
}

int exist_proxy(int key)
{
	if (init_queue() == -1)
		return (-1);
	message.op = 4;
	strcpy(message.queue, queue);
	message.node.key = key;
	if (communication() == -1)
		return (-1);
	if (response != 0)
		printf("Does not exist\n");
	else
		printf("Exists\n");
	return (0);	
}