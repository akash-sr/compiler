#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "../headerFiles/driver.h"
// #include "lexerDef.h"
#include "../headerFiles/setADT.h"
// #include "../headerFiles/stackADT.h"
// #include "../headerFiles/treeADT.h"
// #include "../headerFiles/driver.h"
#include "../definitions/lexerDef.h"
#include "../definitions/hashTableDef.h"
#include <stdio.h>
#include <stdbool.h>

#define RHS_MAX_LENGTH 100
#define NO_MATCHING_RULE -1
#define SYN_RULE -2
#define NUM_OF_RULES 95

typedef enum{
  #include "../nonTerminals.txt"
}ntName;

extern char keyToNT[NUM_OF_NONTERMINALS][MAX_SYMBOL_LENGTH];
extern char keyToToken[NUM_OF_TERMINALS][MAX_SYMBOL_LENGTH];
// extern FILE* parse_tree_file_ptr;

typedef struct{
  union{
    tokenName t;
    ntName nt;
  };
  bool is_terminal;
}symbol;

typedef struct rhsnode{
  symbol sym;
  struct rhsnode *next;
} rhsnode;

typedef struct{
  ntName lhs;
  rhsnode* head;
  rhsnode* tail;
}cell;

int first_set[NUM_OF_TERMINALS][SET_SIZE];
int follow_set[NUM_OF_TERMINALS][SET_SIZE];

cell grammar[NUM_OF_RULES];

hash_table terminal_table;
hash_table non_terminal_table;

int parse_table[NUM_OF_NONTERMINALS][NUM_OF_TERMINALS];
#endif
