
#ifndef PROXY_H
#define PROXY_H

#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "claves.h"
#include "structs_handler.h"

int init_queue();
int communication();
int init_proxy();
int set_value_proxy(int key, char *value1, int N_value2, double *V_value2);
int get_value_proxy(int key, char *value1, int *N_value2, double *V_value2);
int modify_value_proxy(int key, char *value1, int N_value2, double *V_value2);
int delete_key_proxy(int key);
int exist_proxy(int key);
#endif