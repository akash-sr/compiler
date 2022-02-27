#ifndef STACKADT_H
#define STACKADT_H

#include "../definitions/stackADTDef.h"

stack* stack_init();
void push(stack* stack_ptr, void* node);
void* top(stack* stack_ptr);
void* pop(stack* stack_ptr);

#endif