void populate_token_map() {

	FILE *fp = fopen("tokens.txt", "r");
	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *tokens = malloc(sizeof(char) * (fileLength + 1));
  if (tokens == NULL) {
		perror("terminal_string filling failed\n");
		exit(1);
	}

	fread(tokens, sizeof(char), fileLength, fp);
	tokens[fileLength] = '\0';
	fclose(fp);

	char *tok = NULL;

  int key = 0;
	while((tok = strtok(NULL, ", \n"))!=NULL){
		strncpy(keyToToken[key++], tok, MAX_SYMBOL_LENGTH);
	}

	free(tokens);
}
