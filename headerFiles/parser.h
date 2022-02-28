#ifndef PARSER_H
#define PARSER_H

#include "../definitions/parserDef.h"
// #include "lexer.h"
#include "../headerFiles/treeADT.h"
#include <stdio.h>

void grammar_fill(FILE *fp);

symbol get_symbol(char str[]);

void insert_at_end(rhsnode*  *tail, symbol sym);

void print_grammar();

void parser_init();

int rightmost_set_bit(int *num);

void create_parse_table();

void populate_first_sets();

void populate_follow_sets();

void print_first_sets();

void print_follow_sets();

void print_first(ntName a);

void print_rule_fset(int *fset);

void print_parse_table();

void print_parse_tree(char* prefix, tree_node *root, bool isLeft);

// void print_parse_tree_for_tool(tree_node *root, tree_type typ);

void print_rule(int rule_no);

tree_node *parse_input_source_code(FILE *source);

int *get_rule_first_set(rhsnode*  node);

// bool is_superset(ull a[SET_SIZE], ull b[SET_SIZE]);

void free_grammar();

void print_symbol(symbol sym);
extern TOKEN getNextToken(FILE *fp);
// void pretty_print(char *s);
#endif
