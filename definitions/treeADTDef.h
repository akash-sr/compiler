#ifndef TREE_ADT_DEF_H
#define TREE_ADT_DEF_H

#include <stdbool.h>
// #include "../headerFiles/driver.h"
#include "parserDef.h"

// extern char keyToNT[NUM_OF_NONTERMINALS][MAX_SYMBOL_LENGTH];
// extern char keyToToken[NUM_OF_TERMINALS][MAX_SYMBOL_LENGTH];

typedef struct TREENODE{
  struct TREENODE *parent;
  struct TREENODE *sibling;
  struct TREENODE *leftmost_child;
  struct TREENODE *rightmost_child;
  symbol sym;
  // TOKEN token;
  int num_child;
  bool visited;
  // struct {
  //   int start;
  //   int end;
  // }line_nums;
}tree_node;

#endif
