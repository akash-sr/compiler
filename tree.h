/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef TREE_ADT_DEF_H
#define TREE_ADT_DEF_H

#include <stdbool.h>
#include "parserDef.h"


typedef struct NARYTREENODE{
  struct NARYTREENODE *parent;
  struct NARYTREENODE *brother;
  struct NARYTREENODE *leftMostChild;
  struct NARYTREENODE *rightMostChild;
  symbol sym;
  TOKEN token;
  int numOfChildren;
  // int numASTChildren;
}nAryTreeNode;

nAryTreeNode* createTreeNode();
void addChild(nAryTreeNode* parent, nAryTreeNode* child);

#endif
