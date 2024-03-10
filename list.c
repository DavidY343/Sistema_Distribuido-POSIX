#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "list.h"


List messages = NULL;


int init_list(List *l)
{
	*l = NULL;
	return (0);
}

int set(List *l, char *v1, int key, int N, double *v2)
{

    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL)
        return -1;
    strcpy(newNode->v1, v1);
    newNode->key = key;
    newNode->N = N;
    newNode->v2 = (double *)malloc(N * sizeof(double));
    if (newNode->v2 == NULL)
	{
        free(newNode);
        return -1;
    }
    memcpy(newNode->v2, v2, N * sizeof(double)); // MIrar si esto me meto mas datos de los que deberia
    newNode->next = *l;
    *l = newNode;

    return 0;
}

int exist_element(List l, int key)
{
    while (l != NULL)
	{
        if (l->key == key)
            return (0);
        l = l->next;
    }
    return (-1);
}

int get_element(List l, int key, char *value1, int *N_value2, double *V_value2)
{
    while (l != NULL)
	{
        if (l->key == key)
		{
            strcpy(value1, l->v1);
            *N_value2 = l->N;
            memcpy(V_value2, l->v2, l->N * sizeof(double));
            return 0;
        }
        l = l->next;
    }
    return -1;
}

int modify_element(List l, int key, char *new_v1, int new_N, double *new_v2)
{
    while (l != NULL)
	{
        if (l->key == key)
		{
            strcpy(l->v1, new_v1);
            l->N = new_N;
            free(l->v2);
            l->v2 = (double *)malloc(new_N * sizeof(double));
            if (l->v2 == NULL)
                return -1;
            memcpy(l->v2, new_v2, new_N * sizeof(double));
            return 0;
        }
        l = l->next;
    }
    return -1;
}

int delete_element(List *l, int key)
{
    struct Node *prev = NULL;
    struct Node *current = *l;

    while (current != NULL && current->key != key)
	{
        prev = current;
        current = current->next;
    }
    if (current == NULL)
        return -1;
    if (prev == NULL)
        *l = current->next;
    else
        prev->next = current->next;
    free(current->v2);
    free(current);
    return 0;
}

int destroy(List *l)
{
	struct Node *temp;
    while (*l != NULL)
	{
        temp = *l;
        *l = (*l)->next;
        free(temp->v2);
        free(temp);
    }
	return 0;
}
