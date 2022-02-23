typedef enum {
#include "tokens.txt"
} tokenName;

typedef struct token {
  tokenName tok;
  union {
    char id[MAX_LEXEME_LEN];
    int num;
    double rnum;
  };
  int lineNo;
} TOKEN;
