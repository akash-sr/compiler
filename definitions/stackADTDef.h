#ifndef STACKADTDEF_H
#define STACKADTDEF_H
// #include "parserDef.h"
// #include "../headerFiles/treeADT.h"
#define INIT_STACK_SIZE 200

typedef struct{
    void** arr;
    int top;
    int size;
} stack;

#endif
