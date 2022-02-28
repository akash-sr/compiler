#inndef PARSERDEF_H
#define PARSERDEF_H

#include "../headerFiles.h"
#include "../definitions/lexerDef.h"
#include <setADT.h>

#include <stdbool.h>

#define RHS_MAX_LENGTH 100
#define NO_MATCHING_RULE -1
#define SYN_RULE -2

typedef enum{
  #include "../nonTerminals.txt"
}ntName;

extern char keyToNT[NUM_OF_NONTERMINALS][MAX_SYMBOL_LENGTH];
extern FILE* parse_tree_file_ptr;

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

ull first_set[NUM_OF_TERMINALS][SET_SIZE];
ull follow_set[NUM_OF_TERMINALS][SET_SIZE];

cell grammar[NUM_OF_RULES];
#endif
