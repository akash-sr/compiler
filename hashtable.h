/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

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


int calculateHash(char* str);
void initTable(hash_entry* table);
void insertTable(hash_entry* table, char* lexeme, int value);
int searchHashTable(hash_entry* table, char* lexeme);

#endif
