#include "headerFiles/driver.h"
#include "headerFiles/lexer.h"
#include "headerFiles/parser.h"
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
	printf("Enter your choice:");
}

void generateTokenMap() {

	// printf("generatetokenmap ki first line\n");
	FILE *fp = fopen("./tokens.txt", "r");
	// printf("tokens.txt opened successfully\n");

	fseek(fp, 0, SEEK_END);
	// printf("seekend ho gaya\n");
	int fileLength = ftell(fp);
	// printf("ftell ho gaya\n");
	fseek(fp, 0, SEEK_SET);
	// printf("seekset ho gaya\n");


	char *tokens = malloc(sizeof(char) * (fileLength + 1));
	// printf("\n token malloc ho gaya\n");
  	if (tokens == NULL) {
		perror("terminal_string filling failed\n");
		exit(1);
	}

	fread(tokens, sizeof(char), fileLength, fp);
	tokens[fileLength] = '\0';
	// printf("\n tokens ended succesfully \n");
	fclose(fp);

	char *tok = NULL;

  	int key = 0;
	tok = strtok(tokens,", \n");
	strncpy(keyToToken[key++], tok, MAX_SYMBOL_LENGTH);
	while((tok = strtok(NULL, ", \n"))){
		strncpy(keyToToken[key++], tok, MAX_SYMBOL_LENGTH);
	}
	//printf("\n debug statement \n");

	free(tokens);
}

void generateNTMap() {

	// printf("generatetokenmap ki first line\n");
	FILE *fp = fopen("./nonTerminals.txt", "r");
	// printf("tokens.txt opened successfully\n");

	fseek(fp, 0, SEEK_END);
	// printf("seekend ho gaya\n");
	int fileLength = ftell(fp);
	// printf("ftell ho gaya\n");
	fseek(fp, 0, SEEK_SET);
	// printf("seekset ho gaya\n");


	char *nonTerminals = malloc(sizeof(char) * (fileLength + 1));
	// printf("\n token malloc ho gaya\n");
  	if (nonTerminals == NULL) {
		perror("terminal_string filling failed\n");
		exit(1);
	}

	fread(nonTerminals, sizeof(char), fileLength, fp);
	nonTerminals[fileLength] = '\0';
	// printf("\n tokens ended succesfully \n");
	fclose(fp);

	char *nt = NULL;

  int key = 0;
	nt = strtok(nonTerminals,", \n");
	strncpy(keyToNT[key++], nt, MAX_SYMBOL_LENGTH);
	while((nt = strtok(NULL, ", \n"))){
		strncpy(keyToNT[key++], nt, MAX_SYMBOL_LENGTH);
	}
	//printf("\n debug statement \n");

	free(nonTerminals);
}


int main(int argc, char* argv[]){
	// printf("first line\n");
	if(argc!=2){
		perror("Invalid Input!!\n");
		exit(1);
	}

	//printf("generateTokenMap k just pehle\n");

	generateTokenMap();
	generateNTMap();
	//printf("generateTokenMap sahi se ho gaya\n");

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
			case 0:;
				exit(0);
			case 1:;
				char cleanFile[20] = "cleanFile.txt";
				initializeLexer(source);
				removeComments(sourceFile, cleanFile);
				break;
			case 2:;
			  initializeLexer(source);
				getStream(source);
				break;
			case 3:;
				initializeLexer(source);
				parser_init();

				FILE* fp = fopen("grammar.txt", "r");
				if(fp == NULL){
					perror("fopen");
				}

				grammar_fill(fp);
				populate_first_sets();
				populate_follow_sets();

				create_parse_table();
				
				tree_node* ast = parse_input_source_code(source);
				if(ast==NULL){
					printf("Empty parse tree\n");
				}

				char prefix[10000] = "";
			  print_parse_tree(prefix, ast, false);

				// print_parse_tree(ast);
				free_grammar();
				fclose(fp);
				break;
			default:;
				printf("Lite\n");
				break;
		}

	}
}
