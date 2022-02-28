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
