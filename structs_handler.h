#ifndef STRUCTS_HANDLER_H
#define STRUCTS_HANDLER_H
#include "list.h"

struct request{ 
    int         operation;
	struct Node node;
    char        queue[256];
};

#endif
