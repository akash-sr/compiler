void populate_token_map() {

	FILE *fp = fopen("tokens.txt", "r");
	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *tokFile = malloc(sizeof(char) * (fileLength + 1));
  if (tokFile == NULL) {
		perror("terminal_string filling failed\n");
		exit(1);
	}

	fread(tokFile, sizeof(char), fileLength, fp);
	tokFile[fileLength] = '\0';
	fclose(fp);

	char *tokens = NULL;

  int key = 0;
	while((tokens = strtok(NULL, ", \n"))!=NULL){
		strncpy(keyToToken[key++], tokens, MAX_SYMBOL_LENGTH);
	}

	free(tokFile);
}
