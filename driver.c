#include "headerFiles/driver.h"
#include "headerFiles/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void printMenu(){
	printf("\tPress option for the defined task\n");
	printf("0: Exit\n");
	printf("1: Remove Comments\n");
	printf("2: Print Token list\n");
	printf("3: Parse\n");
	printf("4: Time taken\n");
	printf("-------------------------------------\n");
	printf("Enter you choice:");
}

void generateTokenMap() {

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

int main(int argc, char* argv[]){
	if(argc!=2){
		perror("Invalid Input!!\n");
		exit(1);
	}

	generateTokenMap();

	char sourceFile[10];
	strncpy(sourceFile, argv[1], 10);
	FILE *source = fopen(sourceFile, "r");

	if(source==NULL){
		printf("Error opening file\n");
	}

	int choice;

	while(true){
		printMenu();
		scanf("%d", &choice);
		printf("\n");

		switch(choice){
			case 0:
				exit(0);
			case 1:
				char cleanFile[] = "cleanFile.c";
				removeComments(sourceFile, cleanFile);
				break;
			case 2:
				FILE *fp = getStream(source);
				break;
			default:
				printf("Lite\n");
				break;
		}

	}
}
