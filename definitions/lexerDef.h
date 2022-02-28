#ifndef LEXERDEF_H
#define LEXERDEF_H

#include "../headerFiles/hashTable.h"
#include "../headerFiles/driver.h"

#define MAX_BUFFER_SIZE 800
#define MAX_FUNID_LEN 30
#define MAX_TKID_LEN 20

typedef enum {
#include "../tokens.txt"
} tokenName;

// typedef enum{
//   #include "../nonTerminals.txt"
// } ntName;
//yaha par commit kiya hai

typedef struct token {
  tokenName name;
  union {
    char id[MAX_LEX_LEN];
    int num;
    double rnum;
  };
  int line_no;
} TOKEN;

int state;
int extraChars;

hash_entry lookup_table[HASHTABLE_SIZE];
char  twinBuffer[MAX_BUFFER_SIZE];
char lexeme[MAX_LEX_LEN];

int lexeme_begin;
int fwdPtr;
int rounds_completed;
int line_no;

extern char keyToToken[NUM_OF_TERMINALS][MAX_SYMBOL_LENGTH];
extern char keyToNT[NUM_OF_NONTERMINALS][MAX_SYMBOL_LENGTH];

#endif
