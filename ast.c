#include "ast.h"
# include <stdlib.h>
# include <stdio.h>
#include <string.h>
int useful(int tokenClass) {
    switch(tokenClass) {
        case TK_CALL:
        case TK_WITH:
        case TK_PARAMETERS:
        case TK_PARAMETER:
        case TK_SEM:
        case TK_OP:
        case TK_CL:
        case TK_SQL:
        case TK_SQR:
        case TK_COMMA:
        case TK_ENDIF:
        case TK_ENDUNION:
        case TK_ENDRECORD:
        case TK_TYPE:
        case TK_COLON:
        case TK_LIST:
        case TK_END:
        case TK_DEFINETYPE:
        case TK_AS:
        case TK_ENDWHILE:
        case EPSILON:
            return 0;
            break;

        default:
            return 1;
    }
}

void copy(nAryTreeNode* dst,nAryTreeNode* src)
{
    dst->numOfChildren = src->numOfChildren;
    dst->leftMostChild = src->leftMostChild;
    dst->rightMostChild = src->rightMostChild;
    // dst->brother = src->brother;
    dst->sym.isTerminal = src->sym.isTerminal;
    if(dst->sym.isTerminal){
      dst->sym.t = src->sym.t;
    }
    else dst->sym.nt = src->sym.nt;
    dst->token.line_no = src->token.line_no;
    dst->token.name = src->token.name;
    switch (dst->token.name) {
      case TK_NUM: dst->token.num = src->token.num;break;
      case TK_RNUM: dst->token.rnum = src->token.rnum;break;
      default:strncpy(dst->token.id, src->token.id, MAX_LEX_LEN);
    }
}

void buildASTChildren(nAryTreeNode* parent, nAryTreeNode* children){
	nAryTreeNode* temp = children;
	while(temp!=NULL){
		nAryTreeNode* curChild = buildAST(temp);
		if(curChild != NULL){
			addChild(parent, curChild);
		}
		temp = temp->brother;
	}
}

nAryTreeNode* buildAST(nAryTreeNode* root)
{
	if(!root)
		return NULL;

	nAryTreeNode* ast;

	if(root->numOfChildren==0){

      if(!root->sym.isTerminal){
          return NULL;
      }else if(!useful(root->sym.t)){
				return NULL;
      }
			else{
				ast = createTreeNode();
				copy(ast, root);
				return ast;
			}
	}
	ast = createTreeNode();
  ast->sym.isTerminal = root->sym.isTerminal;
    if(ast->sym.isTerminal){
      ast->sym.t = root->sym.t;
    }
    else ast->sym.nt = root->sym.nt;

    ast->token.name = root->token.name;
    switch (ast->token.name) {
      case TK_NUM: ast->token.num = root->token.num;break;
      case TK_RNUM: ast->token.rnum = root->token.rnum;break;
      default:strncpy(ast->token.id, root->token.id, MAX_LEX_LEN);
    }

  //ast me root ko copy karna hai

	buildASTChildren(ast, root->leftMostChild);

	if(ast->numOfChildren==0){
    free(ast);
		return NULL;
	}
	else if(ast->numOfChildren == 1){
    nAryTreeNode * temp=ast->leftMostChild;
		copy(ast, ast->leftMostChild);
    free(temp);
	}
	return ast;
}