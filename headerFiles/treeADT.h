#ifndef TREE_ADT_H
#define TREE_ADT_H

#include "../definitions/treeADTDef.h"

tree_node* create_tree_node();
void add_child(tree_node* parent, tree_node* child);
tree_node* delete_child(tree_node* parent, tree_node* prev, tree_node* child);
tree_node *get_nth_child(tree_node* root, int n);

#endif
