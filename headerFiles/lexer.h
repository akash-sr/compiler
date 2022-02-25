#ifndef LEXERDEF_H
#define LEXER_H

#include "../definitions/lexerDef.h"
#include <stdio.h>
#include <stdlib.h>

void initializeLexer(FILE *source);
void generateLookupTable();
void generateTokenMap();

TOKEN getNextToken(FILE *fp);
TOKEN getToken();
char getChar(FILE *fp);

void fillBuffer(FILE *fp);
void retract(int numOfChars);

tokenName searchKeyword(char *lexeme);

void removeComments(char * sourceFile, char* cleanFile);
FILE* getStream(FILE *source);

#endif
