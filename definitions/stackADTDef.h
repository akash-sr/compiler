#ifndef STACKADTDEF_H
#define STACKADTDEF_H

#define INIT_STACK_SIZE 200

typedef struct{
    void** arr;
    int top;
    int size;
} stack;

#endif