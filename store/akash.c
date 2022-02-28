//parser.c
/*
parseInputSourceCode
printParseTree
*/
tree_node* parse_input_source_code(FILE* source){
  stack* main_stack = stack_init();
  stack* aux_stack = stack_init();
  TOKEN tkn;

  tree_node* root = create_tree_node();
  root->sym->t = TK_DOLLAR;
  root->sym->is_terminal = true;

  tree_node* start = create_tree_node();
  start->sym->nt = program;
  start->sym->is_terminal = false;
  push(main_stack,start);

  tkn = getNextToken(source);

  while(main_stack->size>0){
      tree_node* stTop = (tree_node*)pop(main_stack);
      if(stTop->sym->is_terminal){
        // if match
        if(tkn.name == TK_DOLLAR){
          if(stTop->sym->t == tkn.name){
            //parsing complete
            printf("Prasing Complete\n");
            break;
          }
          else{
            //Error
            perror("Extra characters present in input\n");
            break;
          }
        }
        if(stTop->sym->t == tkn.name){
          //increment input pointer
          tkn = getNextToken(source);
          continue;
        }
        else{
          // error
        }
      }
      else{
        //expand using parse table
        int ruleNo = parse_table[stTop->sym->nt][tkn.name];
        if(ruleNo == NO_MATCHING_RULE){
          //error
        }
        else if(ruleNo == SYN_RULE){
          //error. Enter panic mode
          while(tkn.name!=TK_DOLLAR){
            tkn = getNextToken(source);
            if(set_find_elem(follow_set[stTop->sym->nt], tkn.name))
              break;
          }
          if(tkn.name == TK_DOLLAR){
            printf("Prasing completed\n");
            break;
          }
        }
        else{
          // get the rule
          cell rule = grammar[ruleNo];
          rhsnode* temp = rule.head;

          // expand using that rule
          while(temp!=NULL){
            tree_node* node = create_tree_node();
            node->sym->is_terminal = temp->sym->is_terminal;
            if(node->sym->is_terminal)
              node->sym->t = temp->sym->t;
            else
              node->sym->nt = temp->sym->nt;
            // don't add epsilon to stack
            if(node->sym->is_terminal==false || node->sym->t != EPSILON)
              push(aux_stack, node);
            add_child(stTop, node);
            temp=temp->next;
          }

          // push contents of aux stack to main stack
          /*
          This is done to add the rule in order
          */
          while(aux_stack->size>0){
            tree_node* cur = pop(aux_stack);
            push(main_stack, cur);
          }

        }
      }
  }
  return start;
}


//parser.h
#ifndef PARSER_H
#define PARSER_H

#include "../headerFiles/treeADT.h"
// parserDef.h

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

//treeADTDef.h
#ifndef TREE_ADT_DEF_H
#define TREE_ADT_DEF_H

#include <stdbool.h>
#define "../headerFiles/driver.h"
#define "../definitions/parserDef.h"

extern char keyToNT[NUM_OF_NONTERMINALS][MAX_SYMBOL_LENGTH];
extern char keyToToken[NUM_OF_TERMINALS][MAX_SYMBOL_LENGTH];

typedef struct TREENODE{
  struct TREENODE *parent;
  struct TREENODE *sibling
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
  // newNode->line_nums.start = 0;
  // newNode->line_nums.end = 0;
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
