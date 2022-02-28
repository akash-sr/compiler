#ifndef LEXERDEF_H
#define LEXER_H

#include "../definitions/lexerDef.h"
#include <stdio.h>
#include <stdlib.h>

void initializeLexer(FILE *source);
void generateLookupTable();
extern void generateTokenMap();

extern TOKEN getNextToken(FILE *fp);
extern TOKEN getToken();
char getChar(FILE *fp);

extern void fillBuffer(FILE *fp);
void fillCommentBuffer(FILE* fp);
extern void retract(int numOfChars);

extern tokenName searchKeyword(char *lexeme);

void removeComments(char * sourceFile, char* cleanFile);
void getStream(FILE *source);

#endif
