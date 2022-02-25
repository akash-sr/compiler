#define __STDC_FORMAT_MACROS
#include <inttypes.h>
// strncpy me \0 ke baare me check karna he
int lexeme_begin;
int forward_ptr;
int line_no;
int state;
TOKEN get_next_token(FILE *fp)
{
    char c;
    TOKEN tkn;
    while (1)
    {
        tkn.line_no = line_no;
        switch (state)
        {
        case 0:;
            c = get_char(fp);
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

        case 1:;
            c = get_char(fp);
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

        case 2:;
            retract(1);
            // tkn.name = TK_SPACE;
            lexeme_begin = forward_ptr;
            state = 0;
            break;

        case 3:;
            c = get_char(fp);
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

        case 4:;
            retract(1);
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 5:;
            c = get_char(fp);
            if (isdigit(c))
            {
                state = 7;
            }
            else
            {
                state = 6;
            }
            break;

        case 6:;
            retract(2);
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 7:;
            c = get_char(fp);
            if (isdigit(c))
            {
                state = 9;
            }
            else
            {
                state = 8;
            }
            break;

        case 8:;
            retract(3);
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 9:;
            c = get_char(fp);
            if (c == 'E')
            {
                state = 11;
            }
            else
            {
                state = 10;
            }
            break;

        case 10:;
            retract(1);
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 11:;
            c = get_char(fp);
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
                state = 12;
            }
            break;

        case 12:;
            retract(2);
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 13:;
            c = get_char(fp);
            if (isdigit(c))
            {
                state = 15;
            }
            else
            {
                state = 14;
            }
            break;

        case 14:;
            retract(3);
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 15:;
            c = get_char(fp);
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

        case 16:;
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 17:;
            c = get_char(fp);
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

        case 18:;
            c = get_char(fp);
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

        case 19:;
            c = get_char(fp);
            if (isdigit(c))
            {
                state = 19;
            }
            else
            {
                state = 20;
            }
            break;

        case 20:;
            retract(1);
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 21:;
            c = get_char(fp);
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

        case 22:;
            c = get_char(fp);
            if (c >= 'a' && c <= 'z')
            {
                state = 22;
            }
            else
            {
                state = 23;
            }
            break;

        case 23:;
            retract(1);
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 24:;
            c = get_char(fp);
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

        case 25:;
            c = get_char(fp);
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

        case 26:;
            c = get_char(fp);
            if (c >= '2' && c <= '7')
            {
                state = 26;
            }
            else
            {
                state = 27;
            }
            break;

        case 27:;
            retract(1);
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 28:;
            c = get_char(fp);
            if (c >= 'a' && c <= 'z')
            {
                state = 28;
            }
            else
            {
                state = 29;
            }
            break;

        case 29:;
            retract(1);
            tkn = get_token();
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 30:;
            tkn.name = TK_MUL;
            strncpy(tkn.id.str, "*", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 31:;
            tkn.name = TK_DIV;
            strncpy(tkn.id.str, "/", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 32:;
            tkn.name = TK_NOT;
            strncpy(tkn.id.str, "~", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 33:;
            c = get_char(fp);
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

        case 34:;
            tkn.name = TK_NE;
            strncpy(tkn.id.str, "!=", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 35:;
            tkn.name = TK_COMMENT;
            strncpy(tkn.id.str, "%%", MAX_LEXEME_LEN); // hiw to include % inside "" ?
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 36:;
            tkn.name = TK_SQL;
            strncpy(tkn.id.str, "[", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 37:;
            tkn.name = TK_SQR;
            strncpy(tkn.id.str, "]", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 38:;
            tkn.name = TK_COMMA;
            strncpy(tkn.id.str, ",", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 39:;
            tkn.name = TK_SEM;
            strncpy(tkn.id.str, ";", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;
        case 40:;
            tkn.name = TK_COLON;
            strncpy(tkn.id.str, ":", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;
        case 41:;
            tkn.name = TK_DOT;
            strncpy(tkn.id.str, ".", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 42:;
            c = get_char(fp);
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

        case 43:;
            c = get_char(fp);
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

        case 44:;
            tkn.name = TK_AND;
            strncpy(tkn.id.str, "&&&", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 45:;
            c = get_char(fp);
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

        case 46:;
            c = get_char(fp);
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

        case 47:;
            tkn.name = TK_OR;
            strncpy(tkn.id.str, "@@@", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 48:;
            c = get_char(fp);
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

        case 49:;
            retract(1);
            tkn.name = TK_LT;
            strncpy(tkn.id.str, "<", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 50:;
            c = get_char(fp);
            if (c == '-')
            {
                state = 52;
            }
            else
            {
                state = 51;
            }
            break;

        case 51:;
            retract(2);
            tkn.name = TK_LT;
            strncpy(tkn.id.str, "<", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 52:;
            c = get_char(fp);
            if (c == '-')
            {
                state = 54;
            }
            else
            {
                state = 53;
            }
            break;

        case 53:;
            retract(3);
            tkn.name = TK_LT;
            strncpy(tkn.id.str, "<", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 54:;
            tkn.name = TK_ASSIGNOP;
            strncpy(tkn.id.str, "<---", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 55:;
            tkn.name = TK_LE;
            strncpy(tkn.id.str, "<=", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 56:;
            c = get_char(fp);
            if (c == '=')
            {
                state = 58;
            }
            else
            {
                state = 57;
            }
            break;

        case 57:;
            retract(1);
            tkn.name = TK_GT;
            strncpy(tkn.id.str, ">", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 58:;
            tkn.name = TK_GE;
            strncpy(tkn.id.str, ">=", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 59:;
            c = get_char(fp);
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

        case 60:;
            tkn.name = TK_EQ;
            strncpy(tkn.id.str, "==", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 61:;
            tkn.name = TK_OP;
            strncpy(tkn.id.str, "(", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 62:;
            tkn.name = TK_CL;
            strncpy(tkn.id.str, ")", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 63:;
            tkn.name = TK_PLUS;
            strncpy(tkn.id.str, "+", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 64:;
            tkn.name = TK_MINUS;
            strncpy(tkn.id.str, "-", MAX_LEXEME_LEN);
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;
            break;

        case 65:
            tkn.name = LEX_ERROR1;
            int lex_size = forward_ptr - lexeme_begin;
            if (lex_size < 0)
            {
                lex_size += num_of_rounds * BUFFER_SIZE; // yeh wali chij samjh nahi aayi ki sirf negative me hi kyu kar rha he
                num_of_rounds = 0;
            }
            int last_index = (lex_size < MAX_LEXEME_LEN) ? lex_size : -1;
            if (last_index == -1)
            {
                tkn.name = LEX_ERROR2; // error reporting length exceeded
            }
            else
            {
                lexeme[last_index] = '\0';
                strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
            }
            lexeme_begin = forward_ptr;
            state = 0;
            return tkn;

        default:;
            break;
        }
    }
    return tkn;
}
