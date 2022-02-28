// HASH_SIZE ko HASHTABLE_SIZE likha hai
#ifndef HASHTABLEDEF_H
#define HASHTABLEDEF_H

#define HASHTABLE_SIZE 107
#define MAX_LEX_LEN 100
#define KEY_NOT_FOUND -1
#include <stdbool.h>

typedef struct{
    char lexeme[MAX_LEX_LEN+1];
    bool present;
    void* value;
} hash_entry;

typedef hash_entry hash_table[HASHTABLE_SIZE];

#endif
