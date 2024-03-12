#ifndef STRUCTS_HANDLER_H
#define STRUCTS_HANDLER_H
#include "list.h"

struct request{ 
    int         op;
	char 	v1[MAX_VALUE_LENGTH];
	int 	key;
	int 	N;
	double	*v2;
    char        queue[MAX_VALUE_LENGTH];
};

struct response{ 
    int         op;
	char 	v1[MAX_VALUE_LENGTH];
	int 	key;
	int 	N;
	double	*v2;
    char        queue[MAX_VALUE_LENGTH];
	int error;
};

#endif