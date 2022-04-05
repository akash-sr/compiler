#ifndef AST_H
#define AST_H
    
    # include "parser.h"

    int useful(int tokenClass);
    void copy(nAryTreeNode* dst, nAryTreeNode* src);
    void buildASTChildren(nAryTreeNode* parent, nAryTreeNode* Children);
    nAryTreeNode* buildAST(nAryTreeNode* root);


#endif