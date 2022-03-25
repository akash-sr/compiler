/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/



#include "driver.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void printMenu(){
	printf("\033[0;32m");
	printf("\tPress option for the defined task\n");
	printf("\033[0m");
	printf("0: Exit\n");
	printf("1: Remove Comments\n");
	printf("2: Print Token list\n");
	printf("3: Parse\n");
	printf("4: Time taken\n");
	printf("-------------------------------------\n");
	printf("Enter your choice:");
}

void generateTokenMap() {

	FILE *fp = fopen("./tokens.txt", "r");

	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(fp);

	fseek(fp, 0, SEEK_SET);


	char *tokens = malloc(sizeof(char) * (fileLength + 1));

  	if (tokens == NULL) {
		perror("terminal string filling failed\n");
		exit(1);
	}

	fread(tokens, sizeof(char), fileLength, fp);
	tokens[fileLength] = '\0';

	fclose(fp);

	char *tok = NULL;

  	int key = 0;
	tok = strtok(tokens,", \n");
	strncpy(keyToToken[key++], tok, MAX_SYMBOL_LENGTH);
	while((tok = strtok(NULL, ", \n"))){
		strncpy(keyToToken[key++], tok, MAX_SYMBOL_LENGTH);
	}

	free(tokens);
}

void generateNTMap() {

	FILE *fp = fopen("./nonTerminals.txt", "r");

	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);


	char *nonTerminals = malloc(sizeof(char) * (fileLength + 1));
  	if (nonTerminals == NULL) {
		perror("terminal string filling failed\n");
		exit(1);
	}

	fread(nonTerminals, sizeof(char), fileLength, fp);
	nonTerminals[fileLength] = '\0';
		fclose(fp);

	char *nt = NULL;

  int key = 0;
	nt = strtok(nonTerminals,", \n");
	strncpy(keyToNT[key++], nt, MAX_SYMBOL_LENGTH);
	while((nt = strtok(NULL, ", \n"))){
		strncpy(keyToNT[key++], nt, MAX_SYMBOL_LENGTH);
	}

	free(nonTerminals);
}


int main(int argc, char* argv[]){
	if(argc!=3){
		printf("Invalid Input: Run as ./stage1exe <source> <outfile>\n");
		exit(0);
	}
	printf("Group 10\n");
	printf("(a)Status:\n");
	printf("(b)FIRST and FOLLOW sets automated\n");
	printf("(c)Both lexical and syntax analysis modules implemented\n");
	printf("(d)Modules work with test cases: testcase2.txt, testcase3.txt, testcase4.txt, testcase5.txt\n");
	printf("(e)Lexical analysis works with testcase1.txt and testcase6.txt as well but syntax analysis gives different errors\n");
	char sourceFile[20];
	char outputFile[20];

	strcpy(sourceFile, argv[1]);
	strcpy(outputFile, argv[2]);

	FILE *source, *output, *fp;

	generateTokenMap();
	generateNTMap();
	int choice;

	while(true){
		printMenu();
		scanf("%d", &choice);
		printf("\n");

		switch(choice){
			case 0:;
				exit(0);
			case 1:;
				source = fopen(sourceFile, "r");
				output = fopen(outputFile, "w");
				if(source==NULL || output==NULL){
					printf("Error opening file\n");
					return 0;
				}
				initializeLexer(source);
				removeComments(sourceFile);
				fclose(source);
				fclose(output);
				break;
			case 2:;


				source = fopen(sourceFile, "r");
				output = fopen(outputFile, "w");
				if(source==NULL||output==NULL){
					printf("Error opening file\n");
					return 0;
				}
			  initializeLexer(source);
				getStream(source);
				fclose(source);
				fclose(output);
				break;
			case 3:;

				source = fopen(sourceFile, "r");
				output = fopen(outputFile, "w");
				if(source==NULL||output==NULL){
					printf("Error opening file\n");
					return 0;
				}
				initializeLexer(source);
				initializeParser();
				fp = fopen("grammar.txt", "r");
				if(fp == NULL){
					perror("fopen");
				}
				populateGrammar(fp);
				computeFirstSets();
				computeFollowSets();
				generateParseTable();
				nAryTreeNode* ast = parseInputSourceCode(source);
				if(ast==NULL){
					printf("Empty parse tree\n");
				}

				char prefix[10000] = "";
				printf("\033[0;36m");
				printf("\nPrinting Pre-Order traversal of parse tree on console\n");
				printf("\033[0m");
				printParseTreePreorder(prefix, ast, true);
				printf("\033[0;36m");
				printf("\nStoring In-Order traversal of parse tree in %s\n",outputFile);
				printf("\033[0m");
				char col1[] = "Lexeme CurrentNode";
				char col2[] = "Line no";
				char col3[] = "Token name";
				char col4[] = "ValueIfNumber";
				char col5[] = "ParentNodeSymbol";
				char col6[] = "IsLeafNode";
				char col7[] = "NodeSymbol";
				fprintf(output, "%-30s %-30s %-30s %-30s %-30s %-30s %-30s\n", col1, col2, col3, col4, col5, col6, col7);
				printParseTree(ast, output);
				freeGrammar();
				fclose(fp);
				fclose(source);
				fclose(output);
				break;
			case 4:;
								clock_t start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();
								source = fopen(sourceFile, "r");
								output = fopen(outputFile, "w");
								if(source==NULL||output==NULL){
									printf("Error opening file\n");
									return 0;
								}
								initializeLexer(source);
								initializeParser();

								fp = fopen("grammar.txt", "r");
								if(fp == NULL){
									perror("fopen");
								}

								populateGrammar(fp);
								computeFirstSets();
								computeFollowSets();


								generateParseTable();
								nAryTreeNode* ast2 = parseInputSourceCode(source);
								if(ast2==NULL){
									printf("Empty parse tree\n");
								}

								freeGrammar();
								fclose(fp);
								fclose(source);
								fclose(output);
                end_time = clock();
                total_CPU_time = (double) (end_time - start_time);
                total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
                printf("total CPU time = %f \ntotal CPU time in seconds = %f\n",total_CPU_time,total_CPU_time_in_seconds);
                break;
			default:;
				printf("Lite\n");
				break;
		}

	}
}
