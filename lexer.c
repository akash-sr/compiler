tokenName search_lookup_table(char *lexeme) {
  int key = search_hash_table(lookup_table, lexeme);
  if (key == KEY_NOT_FOUND) {
    return TK_ID;
  }
  else{
    return key;
  }
}

void retract(int numChar) {
  fwdPtr -= numChar;
  if (fwdPtr < 0) {
    fwdPtr += BUFFER_SIZE;
  }
  extraChars += numChar;
}
