//parser.c
/*
parseInputSourceCode
printParseTree
*/

//parserDef.h
// parserDef.h

#inndef PARSERDEF_H
#define PARSERDEF_H

#include "../headerFiles.h"
#include "../definitions/lexerDef.h"
#include <setADT.h>

#include <stdbool.h>

#define RHS_MAX_LENGTH 100
#define NO_MATCHING_RULE -1

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

#endif

//treeADTDef.h
#ifndef TREE_ADT_DEF_H
#define TREE_ADT_DEF_H

#include <stdbool.h>
#define "../headerFiles/driver.h"
#define "../definitions/parserDef.h"

extern char keyToNT[NUM_OF_NONTERMINALS][MAX_SYMBOL_LENGTH];
extern char keyToToken[NUM_OF_TERMINALS][MAX_SYMBOL_LENGTH];

tyepdef struct TREENODE{
  struct TREENODE *parent;
  struct TREENODE *sibling
  struct TREENODE *leftmost_child;
  struct TREENODE *rightmost_child;
  symbol sym;
  TOKEN token;
  int num_child;
  bool visited;
  // struct {
  //   int start;
  //   int end;
  // }line_nums;
}tree_node;

#endif

//treeADT.h
#infdef TREE_ADT_H
#define TREE_ADT_H

#include "../definitions/treeADTDef.h"

tree_node* create_tree_node();
void add_child(tree_node* parent, tree_node* child);
tree_node* delete_child(tree_node* parent, tree_node* prev, tree_node* child);
tree_node *get_nth_child(tree_node* root, int n);

#endif

//treeADT.c

tree_node* create_tree_node(){
  tree_node* newNode = (tree_node*)malloc(sizeof(tree_node));
  if(newNode == NULL){
    perror("Error creating new node..\n");
    exit(0);
  }
  newNode->parent = NULL;
  newNode->sibling = NULL;
  newNode->leftmost_child = NULL;
  newNode->rightmost_child = NULL;
  newNode->num_child = 0;
  newNode->visted = false;
  newNode->line_nums.start = 0;
  newNode->line_nums.end = 0;

  return newNode;
}

void add_child(tree_node* parent, tree_node* child){
  if(parent->num_child==0){
    parent->leftmost_child = child;
    parent->rightmost_child = child;
  }
  else{
    parent->rightmost_child->sibling = child;
    parent->rightmost_child = child;
  }
  parent->num_child = parent->num_child + 1;
  child->parent = parent;
}

tree_node* delete_child(tree_node* parent, tree_node* prev, tree_node* child){
  if(parent == NULL){
    free(child);
    return NULL;
  }

  if(prev!=NULL)
    prev->sibling = child->sibling;

  if(parent->leftmost_child == child)
    parent->leftmost_child = child->sibling;

  if(parent->rightmost_child == child)
    parent->rightmost_child = prev;

  free(child);

  parent->num_child = parent->num_child-1;

  if(prev!=NULL)
    return prev->sibling;
  else
    return parent->leftmost_child;
}
tree_node *get_nth_child(tree_node* parent, int n){
  if(parent==NULL || parent->num_child < n)
    return NULL;

  tree_node *nthChild = parent->leftmost_child;
  int childNo = 1;

  while(childNo<n){
    nthChild = nthChild->sibling;
    childNo++;
  }
  return nthChild;
}
