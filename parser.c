#include "headerFiles/parser.h"
#include "headerFiles/hashTable.h"
#include "headerFiles/stackADT.h"
#include "headerFiles/setADT.h"
#include "headerFiles/lexer.h"
// #include "headerFiles/treeADT.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void parser_init(){
    initTable(terminal_table);
    initTable(non_terminal_table);
    for(int i=0;i<NUM_OF_NONTERMINALS;i++){
        set_init(first_set[i]);
    }
    for(int i=0;i<NUM_OF_TERMINALS;i++){
        insertTable(terminal_table, keyToToken[i], i);
    }
    for(int i=0;i<NUM_OF_NONTERMINALS;i++){
      insertTable(non_terminal_table, keyToNT[i], i);
    }
    for(int i=0;i<NUM_OF_NONTERMINALS;i++){
        for(int j=0;j<NUM_OF_TERMINALS;j++){
            parse_table[i][j] = NO_MATCHING_RULE;
        }
    }
    // num_tree_nodes = 0;
}

void grammar_fill(FILE *fp)
{
    // grammar ek array he (cell type ka).
    // har index par ek grammar rule ki information store he.
    // 1. lhs of grmmar,
    // 2. first symbol on rhs
    // 3. rlast symbol on rhs
    int rule_num = 0;//jab bhi koi file aayegi grammar ki toh uske saare grammars aayege. toh rule_num globally define karne ki jarurat nahi he. isi function me local variable bnakar grammar array ko index kar lege
    char buffer[RHS_MAX_LENGTH];//hum ek iteration me ek hi rule read karege. toh buffer ki size define bhi utni hi ki

    while (fgets(buffer, sizeof(buffer), fp) != NULL) // fgets keeps on reading until new line is encountered or maximum limit of character array. toh ek baar me hum ek hi rule read karege grammar.txt se
    {
        char *sym_read; //yeh strtok dwara tokenize kiye hue symbol ko point karega

        //????strtok ek baar me ek hi token deta he ???????
        sym_read = strtok(buffer, " \n");//pehla symbol tokenize kar diya

        //humne ek hi rule read kiya he, toh iss while loop ke ek iteration me sabse pehla wala symbol lhs ho hoga. baad me koi bhi lhs nahi aayega
        // sabse pehla token toh lhs hoga, jo ki ntName he
        // toh usko assign kar dete he idhar
        //get_symbol symbol return karega. and grammar.lhs bhi "symbol" type ka he
        grammar[rule_num].lhs = get_symbol(sym_read).nt; // symbol me union ke andar 2 field he -> nt/t. since, lhs nt hi hota he, toh symbol.nt kar diya idhar
        grammar[rule_num].head = NULL;
        grammar[rule_num].tail = NULL;
        sym_read = strtok(NULL, " \n");//ab aage wale tokens(symbols) dekhte he

        while (sym_read != NULL)
        {
            symbol sym = get_symbol(sym_read);
            //symbol ko linked list ke end me insert karte jayege
            insert_at_end(&(grammar[rule_num].tail), sym);//rule_num naam ka variable yha kaam ayega grammar array ko index karne ke liye
            if (grammar[rule_num].head == NULL)//hum head ko pehli baari me hi tail ke bara bar kar denge. pehli baari me tail bhi first symbol ko hi point karega. isliye yeh check lgaya he. baad wali iterations me head ko kuch nahi hoga.
            {
                grammar[rule_num].head = grammar[rule_num].tail;
            }
            sym_read = strtok(NULL, " \n");
        }

        // for (int i = 0; sym_read != NULL; i++)
        // {
        //     if (i == 0) // LHS of a production
        //     {
        //          grammar[rule_num].lhs = get_symbol(sym_read).nt;//symbol me union ke andar 2 field he - nt/t. since, lhs nt hi hota he, toh symbol.nt kar diya idhar
        //          grammar[rule_num].head = NULL;
        //          grammar[rule_num].tail = NULL;
        //     }
        //     else
        //     {
        //         symbol sym = get_symbol(sym_read);
        //         insert_at_end(&(grammar[rule_num].tail), sym);
        //         if (grammar[rule_num].head == NULL)
        //         {
        //             grammar[rule_num].head = grammar[rule_num].tail;
        //         }
        //     }
        //     sym_read = strtok(NULL, " \n");
        // }

        rule_num++;//akhri kar humne ek rule padh hi liya pura, ab agli baar next rule padhenge. toh rule number badha diya
    }
}

symbol get_symbol(char* str){
    symbol sym;
    char lexeme[strlen(str)];
    strcpy(lexeme, str);
    if(str[0] >= 'A' && str[0] <= 'Z'){
        sym.is_terminal = true;
        sym.t = search_hash_table(terminal_table, lexeme);
    }
    else{
        sym.is_terminal = false;
        sym.nt = search_hash_table(non_terminal_table, lexeme);
    }
    return sym;
}

void insert_at_end(rhsnode** ptr_tail, symbol sym){
    rhsnode* node = (rhsnode*) malloc(sizeof(rhsnode));
    if(node == NULL){
        perror("insertion at end failed\n");
        exit(1);
    }
    node->sym = sym;
    node->next = NULL;
    if(*ptr_tail != NULL){
        (*ptr_tail)->next = node;
    }
    *ptr_tail = node;
}


set get_rule_first_set(rhsnode* node) {

  set fset = (set)malloc(sizeof(int) * SET_SIZE);

  if (fset == NULL) {
	perror("get rule first set failed\n");
	exit(1);
  }

  set_init(fset);
  rhsnode* temp = node;
  int sym;

  while (temp != NULL) {
	if ((temp->sym).is_terminal == true) {
	  sym = (temp->sym).t;
	  set_add_elem(fset, sym);
	  return fset;
	}

	else {
	  sym = (temp->sym).nt;
	  set_union(fset, fset, first_set[sym]);

	  if (set_find_elem(first_set[sym], EPSILON) == false) // eps not in the nt
	  {
		return fset;
	  } else {
		if (temp->next != NULL) {
		  set_remove_elem(fset, EPSILON);
		}
	  } // end of else - eps present in fset
	}   // end of else - is nt

	temp = temp->next;
  } // end of while - ll traversal
  return fset;
}

// set get_rule_first_set(rhsnode* head){
//
//     //make the set and do the following things
//     rhsnode* temp=head;
//
//     set curr_follow_set=(set)malloc(sizeof(int)*SET_SIZE);
//
//     if(curr_follow_set==NULL){
//         perror("The new set can't be formed in first_set_of_remaining!!");
//         exit(1);
//     }
//
//     set_init(curr_follow_set);
//
//     while(temp!=NULL){
//
//         if((temp->sym).is_terminal==true){
//             set_add_elem(curr_follow_set,(temp->sym).t);
//             return curr_follow_set;
//         }else{
//             //agar non terminal hai phir
//             //what else to do with this
//
//             set_union(curr_follow_set,curr_follow_set,first_set[(temp->sym).nt]);
//
//             // if(temp->next==NULL){
//             //     return curr_follow_set;
//             // }else
//             if(temp->next==NULL){
//                 return curr_follow_set;
//             }
//             else if(set_find_elem(curr_follow_set,EPSILON)==false){
//                 return curr_follow_set;
//             }else{
//                 set_remove_elem(curr_follow_set,EPSILON);
//                 temp=temp->next;
//             }
//
//         }
//
//     }
//
//     return curr_follow_set;
// }

void populate_first_sets(){


    for(int i=NUM_OF_RULES-1;i>=0;i--){

        int curr_non_terminal=grammar[i].lhs;
        rhsnode* head=grammar[i].head;
        rhsnode* temp=head;

        // printf("%s=>{\n",keyToNT[curr_non_terminal]);

        while(temp!=NULL){

            if((temp->sym).is_terminal==true){
                set_add_elem(first_set[curr_non_terminal],(temp->sym).t);
                // printf("%s,",keyToToken[(temp->sym).t]);
                break;
            }else{
                //non terminal hai
                ///agar non terminal hai toh aage jaao

                //now what to do
                set copy_first_set=(set)malloc(sizeof(int)*SET_SIZE);

                for(int j=0;j<SET_SIZE;j++){
                    copy_first_set[j]=first_set[(temp->sym).nt][j];
                }

                if(set_find_elem(copy_first_set,EPSILON)==true){

                    if(temp->next!=NULL){
                        set_remove_elem(copy_first_set,EPSILON);
                        //set_remove_elem(first_set[curr_non_terminal],EPSILON);
                        //epsilon hata ke phir union karo
                    }

                    set_union(first_set[curr_non_terminal],first_set[curr_non_terminal],copy_first_set);

                    temp=temp->next;
                }else{
                  set_union(first_set[curr_non_terminal],first_set[curr_non_terminal],first_set[(temp->sym).nt]);
                  break;
                }


            }



        }

        // printf("}\n");

    }

}

void populate_follow_sets(){


    bool has_change=true;
    set_add_elem(follow_set[program],TK_DOLLAR);
    while(has_change==true){

        has_change=false;
        for(int i=0;i<NUM_OF_RULES;i++){

            int lhs=grammar[i].lhs;
            rhsnode* head=grammar[i].head;
            rhsnode* temp=head;

            while(temp!=NULL){

                if((temp->sym).is_terminal==true){
                    temp=temp->next;
                }else{
                    //agar koi non terminal hai toh phir uska follow_set calculate kar do

                    set old_follow_set=(set)malloc(sizeof(int)*SET_SIZE);
                    set curr_follow_set=get_rule_first_set(temp->next);

                    if(old_follow_set==NULL){
                        perror("old follow set is not formed!!");
                        exit(1);
                    }

                    for(int j=0;j<SET_SIZE;j++){
                        old_follow_set[j]=follow_set[(temp->sym).nt][j];
                    }

                    if(set_find_elem(curr_follow_set,EPSILON)==true){
                        set_remove_elem(curr_follow_set,EPSILON);
                        set_union(curr_follow_set,curr_follow_set,follow_set[lhs]);
                    }

                    if(temp->next==NULL){
                        set_union(curr_follow_set,curr_follow_set,follow_set[lhs]);
                    }

                    // for(int j=0;j<SET_SIZE;j++){
                    //     old_follow_set[j]=follow_set[(temp->sym).nt][j];
                    // }
// hemant ne glti ki
                    set_union(follow_set[(temp->sym).nt],follow_set[(temp->sym).nt],curr_follow_set);

                    for(int j=0;j<SET_SIZE;j++){
                        if(old_follow_set[j]!=follow_set[(temp->sym).nt][j]){
                            has_change=true;
                        }
                    }

                    free(old_follow_set);
                    temp=temp->next;
                }

            }

        }

    }

}

void create_parse_table() {
  for (int i = 0; i < NUM_OF_RULES; i++) {
	ntName nt = grammar[i].lhs;
	set first_set_rhs = get_rule_first_set(grammar[i].head);
  // set first_set_rhs = first_set[nt];
  // printf("%d\n", i);
	for (int j = 0; j < SET_SIZE; j++) {
	  int x = first_set_rhs[j]; // first set
	  while (x>0) {
		int pos = rightmost_set_bit(&x); // position of terminal in first set
		pos += (INT_NUM_BITS * j);
    // if(i==24)
    // printf("%s\n", keyToToken[pos]);
		if (EPSILON == pos) {
		  for (int j = 0; j < SET_SIZE; j++) {
			int x = follow_set[nt][j]; // if epsilon is there in the first set, add follow
			while (x>0) {
			  int pos = rightmost_set_bit(&x);
			  pos += (INT_NUM_BITS * j);
			  if (EPSILON != pos) {
				parse_table[nt][pos] = i;
			  }
			}
		  }
		}
		else {
		  parse_table[nt][pos] = i; // don't consider epsilon as terminal
		}

	  }

	}
  }
}

// int rightmost_set_bit(set num) {
//   int temp = (*num & (*num - 1));
//   int pos = INT_NUM_BITS - 1 - log2(temp ^ *num);
//   *num = temp;
//   return pos;
// }

// void create_parse_table() {
//   for (int i = 0; i < NUM_OF_RULES; i++) {
// 	// printf("Filling r")
// 	ntName nt = grammar[i].lhs;
// 	set rhs_first_set = get_rule_first_set(grammar[i].head);
//
// 	for (int j = 0; j < SET_SIZE; j++) {
// 	  int num = rhs_first_set[j]; // first of lhs
//
// 	  while (num) {
// 		int pos = rightmost_set_bit(&num); // position of terminal in first set
// 		pos += (INT_NUM_BITS * j);
// 		if (EPSILON == pos) {
// 		  for (int j = 0; j < SET_SIZE; j++) {
// 			int num =
// 				follow_set[nt][j]; // if epsilon in first set, add follow also
// 			while (num) {
// 			  int pos = rightmost_set_bit(&num);
// 			  pos += (INT_NUM_BITS * j);
// 			  if (EPSILON != pos) {
// 				parse_table[nt][pos] = i;
// 			  }
// 			}
// 		  } // end of for
// 		}   // end of if - epsilon presence
// 		else {
// 		  parse_table[nt][pos] = i; // don't consider epsilon as terminal
// 		}
// 	  } // end of while - adding elements of first set
// 	}   // end of for - traversing in all subparts of bitstring
//   }     // end of for - travwersal in all rules
// }

int rightmost_set_bit(int* num){
    int temp1 = (*num & (*num -1)); // Unsets rightmost bit of *num
    int temp2 = temp1 ^ (*num); // temp2 contains only the rightmost set bit of *num
    *num = temp1;
    int lg2 = 0;
    while(temp2){
      lg2++;
      temp2>>=1;
    }
    int position = INT_NUM_BITS -lg2;
    return position;
}

tree_node* parse_input_source_code(FILE* source){
  stack* main_stack = stack_init();
  stack* aux_stack = stack_init();
  TOKEN tkn;

  tree_node* root = create_tree_node();
  (root->sym).t = TK_DOLLAR;
  (root->sym).is_terminal = true;

  tree_node* start = create_tree_node();
  (start->sym).nt = program;
  (start->sym).is_terminal = false;
  push(main_stack,start);

  tkn = getNextToken(source);
  while(tkn.name == TK_COMMENT){
    tkn = getNextToken(source);
  }
  while(main_stack->top>0){
      tree_node* stTop = (tree_node*)pop(main_stack);
      if((stTop->sym).is_terminal){
        // if match
        if(tkn.name == TK_DOLLAR){
          if((stTop->sym).t == tkn.name){
            //parsing complete
            printf("Prasing Complete\n");
            break;
          }
          else{
            //Error
            perror("Extra characters present in input\n");
            return start;
          }
        }
        if((stTop->sym).t == tkn.name){
          //increment input pointer
          stTop->token.line_no = tkn.line_no;
          stTop->token.name = tkn.name;
          switch (tkn.name) {
            case TK_NUM: stTop->token.num = tkn.num;break;
            case TK_RNUM: stTop->token.rnum = tkn.rnum;break;
            default:strncpy(stTop->token.id, tkn.id, MAX_LEX_LEN);

          }
          tkn = getNextToken(source);
          while(tkn.name == TK_COMMENT){
            tkn = getNextToken(source);
          }
          continue;
        }
        else if(tkn.name == LEX_ERROR1){
          // error
          perror("Parsing Error 1");
          tkn = getNextToken(source);
          while(tkn.name == TK_COMMENT){
            tkn = getNextToken(source);
          }
          continue;
        }
      }
      else{
        //expand using parse table
        int ruleNo = parse_table[(stTop->sym).nt][tkn.name];
        if(ruleNo == NO_MATCHING_RULE){
          if(set_find_elem(first_set[stTop->sym.nt],EPSILON)==false){
            printf("Stack Top %s, Token %s at %d, Entered panic mode...\n", keyToNT[stTop->sym.nt], keyToToken[tkn.name], tkn.line_no);
          }
          while (set_find_elem(follow_set[stTop->sym.nt], tkn.name) == false) {
      		tkn = getNextToken(source);
      		if (tkn.name == TK_DOLLAR) {
      		  // store_error(tkn.line_no, SYNTACTIC_ERROR,  "Panic Mode Error Recovery Not possible, Partial parse tree formed");
      		  return start;
      		}
      	  }
        }
        // else if(ruleNo == SYN_RULE){
        //   //error. Enter panic mode
        //
        //   while(tkn.name!=TK_DOLLAR){
        //     tkn = getNextToken(source);
        //     while(tkn.name == TK_COMMENT){
        //       tkn = getNextToken(source);
        //     }
        //     if(set_find_elem(follow_set[(stTop->sym).nt], tkn.name))
        //       break;
        //   }
        //   if(tkn.name == TK_DOLLAR){
        //     printf("Prasing completed\n");
        //     break;
        //   }
        // }
        else{
          // get the rule
          cell rule = grammar[ruleNo];
          rhsnode* temp = rule.head;

          // expand using that rule
          while(temp!=NULL){
            tree_node* node = create_tree_node();
            (node->sym).is_terminal = (temp->sym).is_terminal;
            if((node->sym).is_terminal)
              (node->sym).t = (temp->sym).t;
            else
              (node->sym).nt = (temp->sym).nt;
            // don't add epsilon to stack
            if((node->sym).is_terminal==false || (node->sym).t != EPSILON)
              push(aux_stack, node);
            add_child(stTop, node);
            temp=temp->next;
          }

          // push contents of aux stack to main stack
          /*
          This is done to add the rule in order
          */
          while(aux_stack->top>0){
            tree_node* cur = pop(aux_stack);
            push(main_stack, cur);
          }

        }
      }
  }
  return start;
}

// tree_node *parse_input_source_code(FILE *source) {
//
//   stack *main_stack = stack_init();
//   stack *aux_stack = stack_init();
//   tree_node *root = create_tree_node();
//
//   root->sym.nt = program;
//   root->sym.is_terminal = false;
//   push(main_stack, root); // push start symbol on stack
//   TOKEN tkn = getNextToken(source);
//   while(tkn.name == TK_COMMENT){
//                 tkn = getNextToken(source);
//               }
//   while (true) {
// 	// num_tree_nodes++;
// 	tree_node *node = (tree_node *)pop(main_stack);
//   // printf("%s %s\n",keyToToken[tkn.name], (node->sym).is_terminal?keyToToken[(node->sym).t]:keyToNT[(node->sym).nt]);
// 	if ((node != NULL) && (node->sym).is_terminal == true) {
// 	  if (node->sym.t == EPSILON) {
// 		  node->token.name = EPSILON;
// 		  strncpy(node->token.id, "epsilon", MAX_LEX_LEN);
// 		continue;
// 	  }
// 	  if (node->sym.t != tkn.name) // terminal on top of stack does not match
// 								   // with lookhead symbol
// 	  {
// 		// node->token.name = LEX_ERROR;
// 		// char *err_msg = (char*) malloc(sizeof(char) * MAX_ERR_TYPE_STR_LEN);
// 		// snprintf(err_msg, MAX_ERR_TYPE_STR_LEN,  "Expected " ANSI_COLOR_YELLOW "\"%s\"," ANSI_COLOR_RESET " Found " ANSI_COLOR_YELLOW "\"%s\" \n" ANSI_COLOR_RESET,terminal_string[node->sym.t], terminal_string[tkn.name]);
//     //
// 		// store_error(tkn.line_no, SYNTACTIC_ERROR, err_msg);
//
// 		printf("Popping token %s from stack top\n",
// 			   keyToToken[node->sym.t]);
// 		// tkn = get_next_token(source);
// 		node = (tree_node *)pop(main_stack);
// 		if (node == NULL) {
// 		  // store_error(tkn.line_no, SYNTACTIC_ERROR, "Panic Mode Error Recovery Not possible, Partial parse tree formed");
// 		  return root;
// 		}
// 		continue;
// 	  } else { // la token and stack top match
// 		node->token.line_no = tkn.line_no;
// 		node->token.name = tkn.name;
// 		switch (tkn.name) {
// 		case TK_NUM:
// 		  node->token.num = tkn.num;
// 		  break;
//
// 		case TK_RNUM:
// 		  node->token.rnum = tkn.rnum;
// 		  break;
//
// 		default:
// 		  // node->token.id.str = (char *)malloc(sizeof(MAX_LEXEME_LEN));
// 		  strncpy(node->token.id, tkn.id, MAX_LEX_LEN);
// 		}
// 	  }
//
// 	  tkn = getNextToken(source);
// 	  continue;
// 	}
//
// 	if (tkn.name == LEX_ERROR1) {
// 	  // store_error(tkn.line_no, LEXICAL_ERROR, tkn.id.str);
//
// 	  tkn = getNextToken(source);
// 	  push(main_stack, node);
// 	  continue;
// 	}
// 	if (node == NULL) {
// 	  if (tkn.name != TK_DOLLAR) // rule not read completely but stack became empty
// 	  {
// 		// store_error(tkn.line_no, LEXICAL_ERROR, "Extra symbols in the source code");
// 	  } else {
// 		// printf("\nInput source code is now syntactically correct...........\n\n");
// 	  }
// 	  break;
// 	}
//
// 	int rule_no = parse_table[node->sym.nt][tkn.name];
// 	if (rule_no == NO_MATCHING_RULE) {
// 	  // printf("[%s][%s]", non_terminal_string[node->sym.nt],
// 			//  terminal_string[tkn.name]);
// 	  // store_error(tkn.line_no, LEXICAL_ERROR, "No matching rule found in grammar");
//
// 	  // printf("Waiting for an element in follow of " ANSI_COLOR_YELLOW "\"%s\"\n" ANSI_COLOR_RESET, non_terminal_string[node->sym.nt]);
//
// 	  while (set_find_elem(follow_set[node->sym.nt], tkn.name) == false) {
// 		tkn = getNextToken(source);
// 		if (tkn.name == TK_DOLLAR) {
// 		  // store_error(tkn.line_no, SYNTACTIC_ERROR,  "Panic Mode Error Recovery Not possible, Partial parse tree formed");
// 		  return root;
// 		}
// 	  }
// 	  printf("Token \"%s\" found at line number %d\n",
// 			 keyToToken[tkn.name], tkn.line_no);
// 	  // printf(ANSI_COLOR_GREEN "Resuming parsing\n" ANSI_COLOR_RESET);
// 	  continue;
// 	}
// 	cell rule = grammar[rule_no];
// 	rhsnode* rhs_ptr = rule.head;
//
// 	while (rhs_ptr != NULL) {
// 	  tree_node *temp = create_tree_node();
// 	  temp->parent = node;
// 	  temp->sym = rhs_ptr->sym;
// 	  add_child(node, temp);
// 	  push(aux_stack, temp);
// 	  rhs_ptr = rhs_ptr->next;
// 	}
//
// 	tree_node *temp = (tree_node *)pop(aux_stack);
//
// 	while (temp != NULL) {
// 	  push(main_stack, temp);
// 	  temp = (tree_node *)pop(aux_stack);
// 	}
//   } // end of while(true) loop : parsing done
//   return root;
// }

void print_parse_tree(char* prefix, tree_node *root, bool isLeft) {
	if(root==NULL) return;
  printf("%s",prefix);
  if(isLeft){
    printf("|---");
  }
  else{
    printf("'---");
  }
  // printf("%d\n",root->value);
  char node[100];
  if((root->sym).is_terminal){
    strcpy(node, keyToToken[(root->sym).t]);
  }
  else{
    strcpy(node, keyToNT[(root->sym).nt]);
  }
  printf("%s %d\n",node, (root->token).line_no);

  char suf1[] = "|   ";
  char suf2[] = "    ";
  tree_node* cur = root->leftmost_child;
  for(int i=0;i<root->num_child;i++){
    char pref [100];
    strcpy(pref, prefix);
    if(isLeft)
      strcat(pref, suf1);
    else strcat(pref, suf2);
    if(i==0){
      print_parse_tree(pref, root->leftmost_child, true);
    }
    else{
      if(i==root->num_child-1)
        print_parse_tree(pref, cur, false);
      else
        print_parse_tree(pref, cur, true);
    }
    cur = cur->sibling;
  }
}
void print_parse_table() {
  for (int i = 0; i < NUM_OF_NONTERMINALS; i++) {
	for (int j = 0; j < NUM_OF_TERMINALS; j++) {
    if(j!=TK_ASSIGNOP) continue;
	  if (parse_table[i][j] != NO_MATCHING_RULE) {
		printf("parse_table[%s][%s] : ", keyToNT[i], keyToToken[j]);
		print_rule(parse_table[i][j]);
	  }
	}
  }
}

void free_grammar()
{
    int i;
    for (i = 0; i < NUM_OF_RULES; i++) // ek loop me ek grammar rule ko free kar denge
    {
        rhsnode* temp = grammar[i].head; // grammar ek cell type ka array he, jo ki humari grammar store karta he
        // iss grammar me head points to first symbol on rhs of production rule
        // rhsnode* ek struct he jo production rule ka rhs side store karta he(ek linked list ke form me)
        rhsnode* prev;
        while (temp != NULL)
        {
            prev = temp->next; // free karne se pehle uske agle wale symbol ko access kar liya
            free(temp);        // isko free kar diya
            temp = prev;
        }
    }
}

void print_grammar()
{
    for (int i = 0; i < NUM_OF_RULES; i++)
    {
        // print_rule(i); // ek ek karke rule no ke hisab se rules print kar rahe he. print_rule int type ka parameter accept karta he. jo ki rule no he.
        int lhs = grammar[i].lhs;             // grammar cell type ka he, cell.lhs ek ntName naam ka enum he. isliye lhs ek int type define kiya he idhar
        rhsnode* head = grammar[i].head;   // grammar.head rhsnode* type ka he. rhsnode* me symbol type ki field he. toh head->symbol pass kar denge as a parementer to print_symbol
        printf("%s -> ", keyToNT[lhs]); // grammar ka lhs print kar diya
        while (head != NULL)                        // ab grammar ka rhs print karna chalu kiya he
        {
            print_symbol(head->sym); // head grammar ke rhs ki linked list ko point kar rha he. isko use karke uss rule ka rhs print kar sakte he
            printf(" ");
            head = head->next;
        }
        printf("\n");
    }
}

void print_rule(int i){
  int lhs = grammar[i].lhs;             // grammar cell type ka he, cell.lhs ek ntName naam ka enum he. isliye lhs ek int type define kiya he idhar
  rhsnode* head = grammar[i].head;   // grammar.head rhsnode* type ka he. rhsnode* me symbol type ki field he. toh head->symbol pass kar denge as a parementer to print_symbol
  printf("%s -> ", keyToNT[lhs]); // grammar ka lhs print kar diya
  while (head != NULL)                        // ab grammar ka rhs print karna chalu kiya he
  {
      print_symbol(head->sym); // head grammar ke rhs ki linked list ko point kar rha he. isko use karke uss rule ka rhs print kar sakte he
      printf(" ");
      head = head->next;
  }
  printf("\n");
}

void print_symbol(symbol sym)
{                                // symbol yaa toh terminal hoga yaa ntName
    if (sym.is_terminal != true) // agar non_terminal he
    {
        printf("%s", keyToNT[sym.nt]); // ntName ek enum he, toh symbol.nt ek number return karega, jisko  use karke ntName string array ko index kar sakte he
    }
    else // agar terminal he
    {
        printf("%s", keyToToken[sym.t]); // terminal "tokenname" type ka he. tokename ek enum he. Toh sym.t ek number return karega. terminal string ~ keytotoken
    }
}

void print_first_sets(){
  FILE * fp = fopen("first.txt", "w");
  if(fp==NULL){
    perror("fopen error !!");
    return;
  }

    for(int i=0;i<NUM_OF_NONTERMINALS;i++){

        fprintf(fp, "%s => {",keyToNT[i]);

        for(int j=0;j<SET_SIZE;j++){

            for(int k=0;k<INT_NUM_BITS;k++){
                if(set_find_elem(first_set[i],j*INT_NUM_BITS+k)==true){
                    fprintf(fp, "%s ,  ",keyToToken[j * INT_NUM_BITS + k]);
                }
            }

        }

        fprintf(fp,"  }\n");

    }
    fclose(fp);
}

void print_follow_sets(){

  FILE * fp = fopen("follow.txt", "w");
  if(fp==NULL){
    perror("fopen error !!");
    return;
  }
    for(int i=0;i<NUM_OF_NONTERMINALS;i++){

        fprintf(fp,"follow_set of (%s)=>{",keyToNT[i]);

        for(int j=0;j<SET_SIZE;j++){

            for(int k=0;k<INT_NUM_BITS;k++){
                if(set_find_elem(follow_set[i],j*INT_NUM_BITS+k)==true){
                    fprintf(fp,"%s,",keyToToken[j * INT_NUM_BITS + k]);
                }
            }



        }

         fprintf(fp,"}\n");

    }
    fclose(fp);
}
