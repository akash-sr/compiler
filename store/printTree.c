#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct TREENODE{
  struct TREENODE *parent;
  struct TREENODE *sibling;
  struct TREENODE *leftmost_child;
  struct TREENODE *rightmost_child;
  // symbol sym;
  // TOKEN token;
  int value;
  int num_child;
  bool visited;
  // struct {
  //   int start;
  //   int end;
  // }line_nums;
}tree_node;

tree_node* create_tree_node(int val){
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
  newNode->visited = false;
  // newNode->line_nums.start = 0;
  // newNode->line_nums.end = 0;
  newNode->value = val;
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

void print_parse_tree(char* prefix, tree_node *root, bool isLeft) {
	if(root==NULL) return;
  printf("%s",prefix);
  if(isLeft){
    printf("|---");
  }
  else{
    printf("'---");
  }
  printf("%d\n",root->value);
  char suf1[] = "|   ";
  char suf2[] = "    ";
  tree_node* cur = root->leftmost_child;
  for(int i=0;i<root->num_child;i++){
    char pref [100];
    strcpy(pref, prefix);
    if(isLeft)
      strcat(pref, suf1);
    else strcat(pref, suf2);
    if(i==0){
      print_parse_tree(pref, root->leftmost_child, true);
    }
    else{
      if(i==root->num_child-1)
        print_parse_tree(pref, cur, false);
      else
        print_parse_tree(pref, cur, true);
    }
    cur = cur->sibling;
  }
}

int main(){
  tree_node* root = create_tree_node(0);
  tree_node* a = create_tree_node(1);
  tree_node* b = create_tree_node(2);
  tree_node* c = create_tree_node(3);
  tree_node* d = create_tree_node(4);
  tree_node* e = create_tree_node(5);
  tree_node* f = create_tree_node(6);
  tree_node* g = create_tree_node(7);
  tree_node* h = create_tree_node(8);

  add_child(root, a);
  add_child(root, b);
  add_child(a, h);
  add_child(b, c);
  add_child(b, d);
  add_child(b, e);
  add_child(d, f);
  add_child(d, g);

  char prefix[100] = "";
  print_parse_tree(prefix, root, false);
  return 0;
}
