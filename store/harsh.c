// Code in file hashTableDef.h

#ifndef HASHTABLEDEF_H
#define HASHTABLEDEF_H

#include <stdbool.h>
#define TABLE_SIZE 29
#define MAX_LEX_LEN 20

typedef struct{
    char lexeme[MAX_LEX_LEN+1];
    bool present;
} hash_entry;

typedef hash_entry hash_table[TABLE_SIZE];

#endif

// Code in file hashTable.h

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "hashTableDef.h"

int calculate_hash(char* str);
void init_table(hash_entry* table);
void insert_table(hash_entry* table, char* lexeme);
bool search_table(hash_entry* table, char* lexeme);

#endif

// Code in file hashTable.c

#include <stdio.h>
#include <string.h>
#include "hashTableDef.h"

int calculate_hash(char* str){
    long long hash_value = 0;
    int n = strlen(str);
    const long long p = 31;
    const long long PRIME = 1000000007;
    long long prime_pow = 1;
    for(int i=0;i<n;i++){
        hash_value = (hash_value + (str[i] * prime_pow) % PRIME) % PRIME;
        prime_pow = (prime_pow * p) % PRIME;
    }
    return hash_value % TABLE_SIZE;
}

void init_table(hash_entry* table){
    for(int i=0;i<TABLE_SIZE;i++){
        table[i].present = false;
    }
}

void insert_table(hash_entry* table, char* lexeme){
    int hash_value = calculate_hash(lexeme);
    while(table[hash_value].present == true){
        hash_value = (hash_value + 1) % TABLE_SIZE;
    }
    table[hash_value].present = true;
    strncpy(table[hash_value].lexeme, lexeme, MAX_LEX_LEN);
}

bool search_table(hash_entry* table, char* lexeme){
    int hash_value = calculate_hash(lexeme);
    int probe_no = 0;
    while(table[hash_value].present == true && probe_no < TABLE_SIZE){
        if(strcmp(table[hash_value].lexeme, lexeme) == 0){
            return true;
        }
        hash_value = (hash_value + 1) % TABLE_SIZE;
        probe_no++;
    }
    return false;
}

// Code in file lexerDef.h

#ifndef LEXERDEF_H
#define LEXERDEF_H

#include "hashTable.h"

hash_entry lookup_table[TABLE_SIZE];

#endif

// Code in file lexer.h

#ifndef LEXER_H
#define LEXER_H

#include "lexerDef.h"

void populate_lookup_table();

#endif

// Code in file lexer.c

#include <stdio.h>
#include "lexerDef.h"

void populate_lookup_table(){
    insert_table(lookup_table, "with");
    insert_table(lookup_table, "parameters");
    insert_table(lookup_table, "end");
    insert_table(lookup_table, "while");
    insert_table(lookup_table, "union");
    insert_table(lookup_table, "endunion");
    insert_table(lookup_table, "definetype");
    insert_table(lookup_table, "as");
    insert_table(lookup_table, "type");
    insert_table(lookup_table, "_main");
    insert_table(lookup_table, "global");
    insert_table(lookup_table, "parameter");
    insert_table(lookup_table, "list");
    insert_table(lookup_table, "input");
    insert_table(lookup_table, "output");
    insert_table(lookup_table, "int");
    insert_table(lookup_table, "real");
    insert_table(lookup_table, "endwhile");
    insert_table(lookup_table, "if");
    insert_table(lookup_table, "then");
    insert_table(lookup_table, "endif");
    insert_table(lookup_table, "read");
    insert_table(lookup_table, "write");
    insert_table(lookup_table, "return");
    insert_table(lookup_table, "call");
    insert_table(lookup_table, "record");
    insert_table(lookup_table, "endrecord");
    insert_table(lookup_table, "else");
}