void populate_token_map() {

	FILE *fp = fopen("tokens.txt", "r");
	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(file);
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
	token = strtok(tokFile, ", \n");

  int key = 0;
	while((token = strtok(NULL, ", \n"))!=NULL){
		strncpy(keyToToken[key++], token, MAX_SYMBOL_LENGTH);
	}

	free(tokFile);
}
