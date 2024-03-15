
#include "claves.h"
#include "list.h"
#include <stdio.h>
#include <pthread.h>
List messages;

int inited_list = 0;
pthread_mutex_t mutex;

int init()
{
	pthread_mutex_lock(&mutex);
	if (destroy(&messages) == -1)
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
	if (init_list(&messages) == -1)
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
	inited_list = 1;
	pthread_mutex_unlock(&mutex);
	return (0);
}

int set_value(int key, char *value1, int N_value2, double *V_value2)
{
	pthread_mutex_lock(&mutex);
	if (inited_list == 0)
	{
		if (init_list(&messages) == -1)
		{
			pthread_mutex_unlock(&mutex);
			return (-1);
		}
		inited_list = 1;
	}
	imprimir_lista(messages);
	if (exist_element(messages, key) == 0)
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
	if (N_value2 > 32 || N_value2 < 1)
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
	if (set(&messages, value1, key, N_value2, V_value2) == 0)
	{
		pthread_mutex_unlock(&mutex);
		return (0);
	}
	else
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
}

int get_value(int key, char *value1, int *N_value2, double *V_value2)
{
	pthread_mutex_lock(&mutex);
	if (inited_list == 0)
	{
		if (init_list(&messages) == -1)
		{
			pthread_mutex_unlock(&mutex);
			return (-1);
		}
		inited_list = 1;
	}
	if (get_element(messages, key, value1, N_value2, V_value2) == 0)
	{
		pthread_mutex_unlock(&mutex);
		return (0);
	}
	else
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
}

int modify_value(int key, char *value1, int N_value2, double *V_value2)
{
	pthread_mutex_lock(&mutex);
	if (inited_list == 0)
	{
		if (init_list(&messages) == -1)
		{
			pthread_mutex_unlock(&mutex);
			return (-1);
		}
		inited_list = 1;
	}
	if (exist_element(messages, key) == -1)
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
	if (N_value2 > 32 || N_value2 < 1)
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
	if (modify_element(messages, key, value1, N_value2, V_value2) == 0)
	{
		pthread_mutex_unlock(&mutex);
		return (0);
	}
	else
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
}

int delete_key(int key)
{
	pthread_mutex_lock(&mutex);
	if (inited_list == 0)
	{
		if (init_list(&messages) == -1)
		{
			pthread_mutex_unlock(&mutex);
			return (-1);
		}
		inited_list = 1;
	}
	if (exist_element(messages, key) == -1)
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
	if (delete_element(&messages, key) == 0)
	{
		pthread_mutex_unlock(&mutex);
		return (0);
	}
	else
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}	
}

int exist(int key)
{
	pthread_mutex_lock(&mutex);
	if (inited_list == 0)
	{
		if (init_list(&messages) == -1)
		{
			pthread_mutex_unlock(&mutex);
			return (-1);
		}
		inited_list = 1;
	}
	if (exist_element(messages, key) == 0)
	{
		pthread_mutex_unlock(&mutex);
		return (0);
	}
	else
	{
		pthread_mutex_unlock(&mutex);
		return (-1);
	}
}
