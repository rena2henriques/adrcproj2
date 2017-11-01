#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#define STACK_MAX 64000


struct Stack {
    int     data[STACK_MAX];
    int     size;
};
typedef struct Stack Stack;


#endif