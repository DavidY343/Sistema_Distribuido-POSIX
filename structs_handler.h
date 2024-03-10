#ifndef STRUCTS_HANDLER_H
#define STRUCTS_HANDLER_H
#include "list.h"

struct request{ 
    int         op;
	struct Node node;
    char        queue[MAX_VALUE_LENGTH];
};

#endif
