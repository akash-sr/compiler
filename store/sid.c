void fill_buffer(FILE* fp) //this is the function which fills the buffer when needed
{
    int num;
    // in case the forward pointer has reached buffer length or buffer length/2 because of retracting and not because the buffer needs to be filled again, then we should not populate the buffer again.
    if (retract_count !=0)
    {
        return;
    }
    if (fwd_ptr == buffer_length)
    {
        fwd_ptr=0;
        rounds_completed++;
    }
    // num stores the number of characters which are read.
    // the characters which are read are stored in an array of characters named buffer.
    // starting from index buffer/2 or from index 0
    num = fread(&buffer[fwd_ptr],sizeof(char),buffer_length/2,fp);
    if(num == buffer_length/2)
    {
        return;
    }
    else
    {
        buffer[num + fwd_ptr] = EOF;
        return;
    }
}

char get_char(FILE* fp)
{
    if(fwd_ptr==buffer_length||fwd_ptr==buffer_length/2)
    {
        fill_buffer(fp);
    }
    char c = buffer[fwd_ptr];
    int lex_index = (fwd_ptr - lexeme_begin >= 0)?(fwd_ptr-lexeme_begin):(fwd_ptr-lexeme_begin+buffer_length);
    if (lex_index<MAX_LEXEME_LEN)
    {
        lexeme[lex_index] =c;
    }
    fwd_ptr+=1;
    if (extraChars > 0)
        extraChars-=1;
    return c;
}

TOKEN get_token() {

    if (lexeme_begin == buffer_length)
    lexeme_begin = 0;
    TOKEN tkn;

    tkn.line_no = line_no;
    int lex_size = forward_ptr - lexeme_begin;
    if (lex_size < 0) {
    lex_size += rounds_completed * BUFFER_SIZE; //ROUNDS COMPLETED WILL BE 0 OR 1 ONLY BECUASE BUFFER IS TOO LARGE
    rounds_completed = 0;
    }
    int last_index = (lex_size >= MAX_LEXEME_LEN) ? MAX_LEXEME_LEN - 1:lex_size;
    lexeme[last_index] = '\0';

    if(state == 20){
        if (lex_size > MAX_FUNID_LEN) // MAX_ID_LEN is a constant equal to maximum length of an identifier.
        {
            tkn.name = LEX_ERROR2;
            strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
            return tkn;
        }
        token_name name = search_lookup_table(lexeme);
        if(name==TK_ID)
            name = TK_FUNID;
        tkn.name = name;
        strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
        return tkn;
    }
    if(state == 23){
        if (lex_size > MAX_RUID_LEN) // MAX_ID_LEN is a constant equal to maximum length of an identifier.
        {
            tkn.name = LEX_ERROR2;
            strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
            return tkn;
        }
        token_name name = search_lookup_table(lexeme);
        if(name==TK_ID)
            name = TK_RUID;
        tkn.name = name;
        strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
        return tkn;
    }
    if(state == 27){
        if (lex_size > MAX_TKID_LEN) // MAX_ID_LEN is a constant equal to maximum length of an identifier.
        {
            tkn.name = LEX_ERROR2;
            strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
            return tkn;
        }
        token_name name = search_lookup_table(lexeme);
        tkn.name = name;
        strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
        return tkn;
    }
    if(state == 29){
        if (lex_size > MAX_FIELDID_LEN) // MAX_ID_LEN is a constant equal to maximum length of an identifier.
        {
            tkn.name = LEX_ERROR2;
            strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
            return tkn;
        }
        token_name name = search_lookup_table(lexeme);
        if(name==TK_ID)
            name = TK_FIELDID;
        tkn.name = name;
        strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
        return tkn;
    }
//   if ( 29 == state || 20 == state || 23 ==state ||27==state) {
//     if (lex_size > MAX_ID_LEN) // MAX_ID_LEN is a constant equal to maximum length of an identifier.
//     {
//       tkn.name = LEX_ERROR2;
//       strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
//       return tkn;
//     }
//     token_name name = search_lookup_table(lexeme);
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
//      strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
//      return tkn;

  if (4 == state || 6 == state || 8== state) {
    tkn.name = TK_NUM;
    tkn.num = atoi(lexeme);
  }

  if (10== state || 16 == state || 14==state|| 12 == state) {
    tkn.name = RNUM;
    tkn.rnum = atof(lexeme); // atof converts string to double and not float.
  }
  return tkn;
}
