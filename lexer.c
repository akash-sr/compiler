#include "headerFiles/driver.h"
#include "headerFiles/hashTable.h"
#include "headerFiles/lexer.h"

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tokenName searchKeyword(char *lexeme) {
  int key = search_hash_table(lookup_table, lexeme);
  if (key == KEY_NOT_FOUND) {
    return TK_ID;
  }
  else{
    return key;
  }
}

TOKEN getToken() {

    if (lexeme_begin == MAX_BUFFER_SIZE)
    lexeme_begin = 0;
    TOKEN tkn;

    tkn.line_no = line_no;
    int lex_size = fwdPtr - lexeme_begin;
    if (lex_size < 0) {
    lex_size += rounds_completed * MAX_BUFFER_SIZE; //ROUNDS COMPLETED WILL BE 0 OR 1 ONLY BECUASE BUFFER IS TOO LARGE
    rounds_completed = 0;
    }
    int last_index = (lex_size >= MAX_LEX_LEN) ? MAX_LEX_LEN - 1:lex_size;
    lexeme[last_index] = '\0';

    if(state == 20){
        if (lex_size > MAX_FUNID_LEN) // MAX_ID_LEN is a constant equal to maximum length of an identifier.
        {
            tkn.name = LEX_ERROR2;
            strncpy(tkn.id, lexeme, MAX_LEX_LEN);
            return tkn;
        }
        tokenName name = searchKeyword(lexeme);
        if(name==TK_ID)
            name = TK_FUNID;
        tkn.name = name;
        strncpy(tkn.id, lexeme, MAX_LEX_LEN);
        return tkn;
    }
    if(state == 23){
        // if (lex_size > MAX_RUID_LEN) // MAX_ID_LEN is a constant equal to maximum length of an identifier.
        // {
        //     tkn.name = LEX_ERROR2;
        //     strncpy(tkn.id, lexeme, MAX_LEX_LEN);
        //     return tkn;
        // }
        tokenName name = searchKeyword(lexeme);
        if(name==TK_ID)
            name = TK_RUID;
        tkn.name = name;
        strncpy(tkn.id, lexeme, MAX_LEX_LEN);
        return tkn;
    }
    if(state == 27){
        if (lex_size > MAX_TKID_LEN) // MAX_ID_LEN is a constant equal to maximum length of an identifier.
        {
            tkn.name = LEX_ERROR2;
            strncpy(tkn.id, lexeme, MAX_LEX_LEN);
            return tkn;
        }
        tokenName name = searchKeyword(lexeme);
        tkn.name = name;
        strncpy(tkn.id, lexeme, MAX_LEX_LEN);
        return tkn;
    }
    if(state == 29){
        // if (lex_size > MAX_FIELDID_LEN) // MAX_ID_LEN is a constant equal to maximum length of an identifier.
        // {
        //     tkn.name = LEX_ERROR2;
        //     strncpy(tkn.id, lexeme, MAX_LEX_LEN);
        //     return tkn;
        // }
        tokenName name = searchKeyword(lexeme);
        if(name==TK_ID)
            name = TK_FIELDID;
        tkn.name = name;
        strncpy(tkn.id, lexeme, MAX_LEX_LEN);
        return tkn;
    }
//   if ( 29 == state || 20 == state || 23 ==state ||27==state) {
//     if (lex_size > MAX_ID_LEN) // MAX_ID_LEN is a constant equal to maximum length of an identifier.
//     {
//       tkn.name = LEX_ERROR2;
//       strncpy(tkn.id, lexeme, MAX_LEX_LEN);
//       return tkn;
//     }
//     tokenName name = searchKeyword(lexeme);
//     if(name==TK_ID)
//     {
//         if(state==20)
//         {
//             name = TK_FUNID;
//         }
//         if(state==23)
//         {
//             name = TK_RUID;
//         }
//         if(state==29)
//         {
//             name = TK_FIELDID;
//         }
//     }
//      tkn.name = name;
//      strncpy(tkn.id, lexeme, MAX_LEX_LEN);
//      return tkn;

  if (4 == state || 6 == state || 8== state) {
    tkn.name = TK_NUM;
    tkn.num = atoi(lexeme);
  }

  if (10== state || 16 == state || 14==state|| 12 == state) {
    tkn.name = TK_RNUM;
    tkn.rnum = atof(lexeme); // atof converts string to double and not float.
  }
  return tkn;
}

void retract(int numChar) {
  fwdPtr -= numChar;
  if (fwdPtr < 0) {
    fwdPtr += MAX_BUFFER_SIZE;
  }
  extraChars += numChar;
}

void generateLookupTable(){
    insertTable(lookup_table, "with", TK_WITH);
    insertTable(lookup_table, "parameters", TK_PARAMETERS);
    insertTable(lookup_table, "end", TK_END);
    insertTable(lookup_table, "while", TK_WHILE);
    insertTable(lookup_table, "union", TK_UNION);
    insertTable(lookup_table, "endunion", TK_ENDUNION);
    insertTable(lookup_table, "definetype", TK_DEFINETYPE);
    insertTable(lookup_table, "as", TK_AS);
    insertTable(lookup_table, "type", TK_TYPE);
    insertTable(lookup_table, "_main", TK_MAIN);
    insertTable(lookup_table, "global", TK_GLOBAL);
    insertTable(lookup_table, "parameter", TK_PARAMETER);
    insertTable(lookup_table, "list", TK_LIST);
    insertTable(lookup_table, "input", TK_INPUT);
    insertTable(lookup_table, "output", TK_OUTPUT);
    insertTable(lookup_table, "int", TK_INT);
    insertTable(lookup_table, "real", TK_REAL);
    insertTable(lookup_table, "endwhile", TK_ENDWHILE);
    insertTable(lookup_table, "if", TK_IF);
    insertTable(lookup_table, "then", TK_THEN);
    insertTable(lookup_table, "endif", TK_ENDIF);
    insertTable(lookup_table, "read", TK_READ);
    insertTable(lookup_table, "write", TK_WRITE);
    insertTable(lookup_table, "return", TK_RETURN);
    insertTable(lookup_table, "call", TK_CALL);
    insertTable(lookup_table, "record", TK_RECORD);
    insertTable(lookup_table, "endrecord", TK_ENDRECORD);
    insertTable(lookup_table, "else", TK_ELSE);
}

void fillBuffer(FILE* fp) //this is the function which fills the buffer when needed
{
    int num;
    // in case the forward pointer has reached buffer length or buffer length/2 because of retracting and not because the buffer needs to be filled again, then we should not populate the buffer again.
    if (extraChars !=0)
    {
        return;
    }
    if (fwdPtr == MAX_BUFFER_SIZE)
    {
        fwdPtr=0;
        rounds_completed++;
    }
    // num stores the number of characters which are read.
    // the characters which are read are stored in an array of characters named buffer.
    // starting from index buffer/2 or from index 0
    num = fread(&twinBuffer[fwdPtr],sizeof(char),MAX_BUFFER_SIZE/2,fp);
    if(num == MAX_BUFFER_SIZE/2)
    {
        return;
    }
    else
    {
        twinBuffer[num + fwdPtr] = EOF;
        return;
    }
}

void initializeLexer(FILE *source) {
  initTable(lookup_table);
  generateLookupTable();

  state = 0;
  lexeme_begin = 0;
  fwdPtr = 0;
  extraChars = 0;
  line_no = 1;
  rounds_completed = 0;

  int num = fseek(source, 0, SEEK_SET); // go back to start of source code file
  fillBuffer(source);
}

char getChar(FILE* fp)
{
    if(fwdPtr==MAX_BUFFER_SIZE||fwdPtr==MAX_BUFFER_SIZE/2)
    {
        fillBuffer(fp);
    }
    char c = twinBuffer[fwdPtr];
    int lex_index = (fwdPtr - lexeme_begin >= 0)?(fwdPtr-lexeme_begin):(fwdPtr-lexeme_begin+MAX_BUFFER_SIZE);
    if (lex_index<MAX_LEX_LEN)
    {
        lexeme[lex_index] =c;
    }
    fwdPtr+=1;
    if (extraChars > 0)
        extraChars-=1;
    return c;
}

// strncpy me \0 ke baare me check karna he
// int lexeme_begin;
// int fwdPtr;
// int line_no;
// int state;
TOKEN getNextToken(FILE *fp)
{
    char c;
    TOKEN tkn;
    while (1)
    {
        tkn.line_no = line_no;
        switch (state)
        {
        case 0:
            c = getChar(fp);
            if (c == ' ' || c == '\n' || c == '\t')
            {
                // tkn.num = c;
                if ('\n' == c)
                {
                    line_no++;
                }
                state = 1;
            }
            else if (isdigit(c))
            {
                state = 3;
            }
            else if (c == '_')
            {
                state = 17;
            }
            else if (c == '#')
            {
                state = 21;
            }
            else if (c == 'b' || c == 'c' || c == 'd')
            {
                state = 24;
            }
            else if (c == 'a' || (c >= 'e' && c <= 'z'))
            {
                state = 28;
            }
            else if (c == '*')
            {
                state = 30;
            }
            else if (c == '/')
            {
                state = 31;
            }
            else if (c == '~')
            {
                state = 32;
            }
            else if (c == '!')
            {
                state = 33;
            }
            else if (c == '%')
            {
                state = 35;
            }
            else if (c == '[')
            {
                state = 36;
            }
            else if (c == ']')
            {
                state = 37;
            }
            else if (c == ',')
            {
                state = 38;
            }
            else if (c == ';')
            {
                state = 39;
            }
            else if (c == ':')
            {
                state = 40;
            }
            else if (c == '.')
            {
                state = 41;
            }
            else if (c == '&')
            {
                state = 42;
            }
            else if (c == '@')
            {
                state = 45;
            }
            else if (c == '<')
            {
                state = 48;
            }
            else if (c == '>')
            {
                state = 56;
            }
            else if (c == '=')
            {
                state = 59;
            }
            else if (c == '(')
            {
                state = 61;
            }
            else if (c == ')')
            {
                state = 62;
            }
            else if (c == '+')
            {
                state = 63;
            }
            else if (c == '-')
            {
                state = 64;
            }
            else if (EOF == c)
            {
                tkn.name = TK_DOLLAR;
                return tkn;
            }
            else
            {
                state = 65;
            }
            break;

        case 1:
            c = getChar(fp);
            if (c == ' ' || c == '\n' || c == '\t')
            {
                if (c == '\n')
                    line_no++;
                state = 1;
            }
            else
            {
                state = 2;
            }
            break;

        case 2:
            retract(1);
            // tkn.name = TK_SPACE;
            lexeme_begin = fwdPtr;
            state = 0;
            break;

        case 3:
            c = getChar(fp);
            if (isdigit(c))
            {
                state = 3;
            }
            else if (c == '.')
            {
                state = 5;
            }
            else
            {
                state = 4;
            }
            break;

        case 4:
            retract(1);
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 5:
            c = getChar(fp);
            if (isdigit(c))
            {
                state = 7;
            }
            else
            {
                retract(1);
                state = 65;
                // state = 6;
            }
            break;

        case 6:
            retract(2);
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 7:
            c = getChar(fp);
            if (isdigit(c))
            {
                state = 9;
            }
            else
            {
                retract(1);
                state = 65;
                // state = 8;
            }
            break;

        case 8:
            retract(3);
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 9:
            c = getChar(fp);
            if (c == 'E')
            {
                state = 11;
            }
            else
            {
                state = 10;
            }
            break;

        case 10:
            retract(1);
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 11:
            c = getChar(fp);
            if (isdigit(c))
            {
                state = 15;
            }
            else if (c == '+' || c == '-')
            {
                state = 13;
            }
            else
            {
                retract(1);
                state = 65;
                // state = 12;
            }
            break;

        case 12:
            retract(2);
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 13:
            c = getChar(fp);
            if (isdigit(c))
            {
                state = 15;
            }
            else
            {
                retract(1);
                state = 65;
                // state = 14;
            }
            break;

        case 14:
            retract(3);
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 15:
            c = getChar(fp);
            if (isdigit(c))
            {
                state = 16;
            }
            else
            {
                retract(1);
                state = 65;
            }
            break;

        case 16:
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 17:
            c = getChar(fp);
            if (isalpha(c))
            {
                state = 18;
            }
            else
            {
                retract(1);
                state = 65;
            }
            break;

        case 18:
            c = getChar(fp);
            if (isalpha(c))
            {
                state = 18;
            }
            else if (isdigit(c))
            {
                state = 19;
            }
            else
            {
                state = 20;
            }
            break;

        case 19:
            c = getChar(fp);
            if (isdigit(c))
            {
                state = 19;
            }
            else
            {
                state = 20;
            }
            break;

        case 20:
            retract(1);
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 21:
            c = getChar(fp);
            if (c >= 'a' && c <= 'z')
            {
                state = 22;
            }
            else
            {
                retract(1);
                state = 65;
            }
            break;

        case 22:
            c = getChar(fp);
            if (c >= 'a' && c <= 'z')
            {
                state = 22;
            }
            else
            {
                state = 23;
            }
            break;

        case 23:
            retract(1);
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 24:
            c = getChar(fp);
            if (c >= '2' && c <= '7')
            {
                state = 25;
            }
            else if (c >= 'a' && c <= 'z')
            {
                state = 28;
            }
            else
            {
                state = 29;
            }
            break;

        case 25:
            c = getChar(fp);
            if (c == 'b' || c == 'c' || c == 'd')
            {
                state = 25;
            }
            else if (c >= '2' && c <= '7')
            {
                state = 26;
            }
            else
            {
                state = 27;
            }
            break;

        case 26:
            c = getChar(fp);
            if (c >= '2' && c <= '7')
            {
                state = 26;
            }
            else
            {
                state = 27;
            }
            break;

        case 27:
            retract(1);
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 28:
            c = getChar(fp);
            if (c >= 'a' && c <= 'z')
            {
                state = 28;
            }
            else
            {
                state = 29;
            }
            break;

        case 29:
            retract(1);
            tkn = getToken();
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 30:
            tkn.name = TK_MUL;
            strncpy(tkn.id, "*", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 31:
            tkn.name = TK_DIV;
            strncpy(tkn.id, "/", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 32:
            tkn.name = TK_NOT;
            strncpy(tkn.id, "~", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 33:
            c = getChar(fp);
            if (c == '=')
            {
                state = 34;
            }
            else
            {
                retract(1);
                state = 65;
            }
            break;

        case 34:
            tkn.name = TK_NE;
            strncpy(tkn.id, "!=", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 35:;
            char d = getChar(fp);
            while((d!='\n') && (d!=EOF)){
                d = getChar(fp);
            }
            retract(1);
            char ch = '%';
            char str[2];
            str[0] = ch;
            str[1] = '\0';
            tkn.name = TK_COMMENT;
            strncpy(tkn.id, str, MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 36:
            tkn.name = TK_SQL;
            strncpy(tkn.id, "[", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 37:
            tkn.name = TK_SQR;
            strncpy(tkn.id, "]", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 38:
            tkn.name = TK_COMMA;
            strncpy(tkn.id, ",", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 39:
            tkn.name = TK_SEM;
            strncpy(tkn.id, ";", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;
        case 40:
            tkn.name = TK_COLON;
            strncpy(tkn.id, ":", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;
        case 41:
            tkn.name = TK_DOT;
            strncpy(tkn.id, ".", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 42:
            c = getChar(fp);
            if (c == '&')
            {
                state = 43;
            }
            else
            {
                retract(1);
                state = 65;
            }
            break;

        case 43:
            c = getChar(fp);
            if (c == '&')
            {
                state = 44;
            }
            else
            {
                retract(1);
                state = 65;
            }
            break;

        case 44:
            tkn.name = TK_AND;
            strncpy(tkn.id, "&&&", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 45:
            c = getChar(fp);
            if (c == '@')
            {
                state = 46;
            }
            else
            {
                retract(1);
                state = 65;
            }
            break;

        case 46:
            c = getChar(fp);
            if (c == '@')
            {
                state = 47;
            }
            else
            {
                retract(1);
                state = 65;
            }
            break;

        case 47:
            tkn.name = TK_OR;
            strncpy(tkn.id, "@@@", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 48:
            c = getChar(fp);
            if (c == '-')
            {
                state = 50;
            }
            else if (c == '=')
            {
                state = 55;
            }
            else
            {
                state = 49;
            }
            break;

        case 49:
            retract(1);
            tkn.name = TK_LT;
            strncpy(tkn.id, "<", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 50:
            c = getChar(fp);
            if (c == '-')
            {
                state = 52;
            }
            else
            {
                retract(1);
                state = 65;
                // state = 51;
            }
            break;

        case 51:
            retract(2);
            tkn.name = TK_LT;
            strncpy(tkn.id, "<", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 52:
            c = getChar(fp);
            if (c == '-')
            {
                state = 54;
            }
            else
            {
                retract(1);
                state = 65;
                // state = 53;
            }
            break;

        case 53:
            retract(3);
            tkn.name = TK_LT;
            strncpy(tkn.id, "<", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 54:
            tkn.name = TK_ASSIGNOP;
            strncpy(tkn.id, "<---", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 55:
            tkn.name = TK_LE;
            strncpy(tkn.id, "<=", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 56:
            c = getChar(fp);
            if (c == '=')
            {
                state = 58;
            }
            else
            {
                state = 57;
            }
            break;

        case 57:
            retract(1);
            tkn.name = TK_GT;
            strncpy(tkn.id, ">", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 58:
            tkn.name = TK_GE;
            strncpy(tkn.id, ">=", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 59:
            c = getChar(fp);
            if (c == '=')
            {
                state = 60;
            }
            else
            {
                retract(1);
                state = 65;
            }
            break;

        case 60:
            tkn.name = TK_EQ;
            strncpy(tkn.id, "==", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 61:
            tkn.name = TK_OP;
            strncpy(tkn.id, "(", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 62:
            tkn.name = TK_CL;
            strncpy(tkn.id, ")", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 63:
            tkn.name = TK_PLUS;
            strncpy(tkn.id, "+", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 64:
            tkn.name = TK_MINUS;
            strncpy(tkn.id, "-", MAX_LEX_LEN);
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        case 65:
            tkn.name = LEX_ERROR1;
            strncpy(tkn.id, lexeme, MAX_LEX_LEN);
            int lex_size = fwdPtr - lexeme_begin;
            if (lex_size < 0)
            {
                lex_size += rounds_completed * MAX_BUFFER_SIZE; // yeh wali chij samjh nahi aayi ki sirf negative me hi kyu kar rha he
                rounds_completed = 0;
            }
            int last_index = (lex_size < MAX_LEX_LEN) ? lex_size : -1;
            if (last_index == -1)
            {
                tkn.name = LEX_ERROR2; // error reporting length exceeded
            }
            else
            {
                lexeme[last_index] = '\0';
                strncpy(tkn.id, lexeme, MAX_LEX_LEN);
            }
            lexeme_begin = fwdPtr;
            state = 0;
            return tkn;
            break;

        default:
            break;
        }
    }
    return tkn;
}

void getStream(FILE *source) {
  TOKEN tkn;
  FILE * fp = fopen("output.txt", "w");
  if(fp==NULL){
    perror("fopen error !!");
    return;
  }

  // printf("LINE_NUMBER", "LEXEME", "tokenName");

  if (source == NULL) {
    perror("source null : print token stream\n");
    return;
  }

  while (true) {
    tkn = getNextToken(source);
    if (tkn.name == TK_DOLLAR) {
      break;
    }
    else {
      if (tkn.name == LEX_ERROR1) {
        // printf("==========================================================\n");
        // printf("%-15d  |  %-20s  |  %-20s\n", tkn.line_no, tkn.id,"This is not the valid lexeme");
        // printf("==========================================================\n");
        fprintf(fp, "Line no. %-15d Unknown pattern <%s> \n", tkn.line_no, lexeme);
      }
      else if(tkn.name == LEX_ERROR2){
        // printf("==========================================================\n");
        // printf("%-15d  |  %-20s  |  %-20s\n", tkn.line_no, tkn.id,"Length exceeded");
        // printf("==========================================================\n");
        fprintf(fp, "Line no. %-15d Variable Identifier is longer than the prescribed length of 20 characters.\n", tkn.line_no);
      }
      else {
          // fprintf(fp,"%-15d  |  ", tkn.line_no);
          switch (tkn.name) {
            case TK_NUM:;
              // fprintf(fp,"%-20d  |  ", tkn.num);
              fprintf(fp, "Line no. %-15d Lexeme %-20d TOKEN %-20s\n", tkn.line_no, tkn.num, keyToToken[tkn.name]);
              break;
            case TK_RNUM:;
              // fprintf(fp,"%-20f  |  ", tkn.rnum);
              fprintf(fp, "Line no. %-15d Lexeme %-20s TOKEN %-20s\n", tkn.line_no, lexeme, keyToToken[tkn.name]);
              break;
            // case TK_FIELDID:
            //   fprintf(fp,"%s  |  ", tkn.id);
            //   break;
            // case TK_FUNID:
            //   fprintf(fp,"%s  |  ", tkn.id);
            //   break;
            // case TK_RUID:
            //   fprintf(fp,"%s  |  ", tkn.id);
            //   break;
            default:;
              fprintf(fp, "Line no. %-15d Lexeme %-20s TOKEN %-20s\n", tkn.line_no, tkn.id, keyToToken[tkn.name]);
              break;
          }
          // fprintf(fp,"%-20s\n", keyToToken[tkn.name]);
          // fprintf(fp, "%s", );
      }
    }
  } // end of while
  fclose(fp);
}
// Hemant's code
// int MAX_BUFFER_SIZE=50;
// int fwdPtr=50;
// int extraChars=0;
// char twinBuffer[50];


// void retract(int numOfChars) {
//   fwdPtr -= numOfChars;
//   if (fwdPtr < 0) {
//     fwdPtr += MAX_BUFFER_SIZE;
//   }
//   extraChars += numOfChars;
// }

void fillCommentBuffer(FILE* fp) //this is the function which fills the buffer when needed
{
    int num;
    if (extraChars !=0){
        return;
    }
    if (fwdPtr == MAX_BUFFER_SIZE)
    {
        fwdPtr=0;
        // rounds_completed++;
    }
    // num stores the numbmer of characters which are read.
    // the characters which are read are stored in an array of characters named buffer. starting from index buffer/2 or from index 0
    num = fread(&twinBuffer[fwdPtr],1,MAX_BUFFER_SIZE/2,fp);
    if(num == MAX_BUFFER_SIZE/2)
    {
        return;
    }
    twinBuffer[num + fwdPtr] = EOF; // ?????????? how come he is treating EOF as a character in C????????
}


char getNextChar(FILE* fp)
{
    if(fwdPtr==MAX_BUFFER_SIZE||fwdPtr==MAX_BUFFER_SIZE/2)
    {
        fillCommentBuffer(fp);
    }
    char c = twinBuffer[fwdPtr];
    // int lex_index = (fwdPtr -lexeme_begin >= 0)?(fwdPtr-lexeme_begin):(fwdPtr-lexeme_begin+MAX_BUFFER_SIZE);
    // if (lex_index<MAX_LEX_LEN)
    // {
    //     lexeme[lex_index] =c;
    // }
    fwdPtr+=1;
    if (extraChars > 0)
        extraChars--;
    return c;
}


void removeComments(char * sourceFile, char* cleanFile){


    int fwdPtr=MAX_BUFFER_SIZE;
    FILE * fptr=fopen(sourceFile,"r");
    FILE * cptr=fopen(cleanFile,"w");

    if(fptr==NULL || cptr==NULL){
        printf("Files can't be opened try again");
        exit(1);
    }else{
        printf("Files Opened\n");
    }


    //printf("%d\n",1);

    // char buff[255];
    char c= getNextChar(fptr);   //fgetc ke badle getChar daalna padega kyoki waha hum chahenge ki buffer me pehle read kar le phir hum kare
    while (c!= EOF)
    {

        if(c=='%'){

            while((c!='\n') && (c != EOF)){
                c=getNextChar(fptr);
            }
            if(c == '\n'){
                fputc(c, cptr);
            }
            else{
                break;
            }

        }else{
            fputc(c,cptr);
        }
        c=getNextChar(fptr);

    }

    fclose(fptr);
    fclose(cptr);

}


// int main(){
//
//
//     char sourceFile[20];
//
//     scanf("%s",sourceFile);
//
//     // printf("%s\n",sourceFile);
//
//     remove_comments(sourceFile);
//
//     printf("%s\n",sourceFile);
//
//     return 0;
// }

// void fillBuffer(FILE* fp) //this is the function which fills the buffer when needed
// {
//     int num;
//     // in case the forward pointer has reached buffer length or buffer length/2 because of retracting and not because the buffer needs to be filled again, then we should not populate the buffer again.
//     if (extraChars !=0)
//     {
//         return;
//     }
//     if (fwdPtr == MAX_BUFFER_SIZE)
//     {
//         fwdPtr=0;
//         rounds_completed++;
//     }
//     // num stores the number of characters which are read.
//     // the characters which are read are stored in an array of characters named buffer.
//     // starting from index buffer/2 or from index 0
//     num = fread(&twinBuffer[fwdPtr],sizeof(char),MAX_BUFFER_SIZE/2,fp);
//     if(num == MAX_BUFFER_SIZE/2)
//     {
//         return;
//     }
//     else
//     {
//         twinBuffer[num + fwdPtr] = EOF;
//         return;
//     }
// }
