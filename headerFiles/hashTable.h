#ifndef HASHTABLE_H
#define HASHTABLE_H


#include "../definitions/hashTableDef.h"
#include <stdbool.h>

int calculateHash(char* str);
void initTable(hash_entry* table);
void insertTable(hash_entry* table, char* lexeme);
bool searchTable(hash_entry* table, char* lexeme);

#endif
