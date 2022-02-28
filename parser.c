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


set get_rule_first_set(rhsnode* head){

    //make the set and do the following things
    rhsnode* temp=head;

    set curr_follow_set=(set)malloc(sizeof(int)*SET_SIZE);

    if(curr_follow_set==NULL){
        perror("The new set can't be formed in first_set_of_remaining!!");
        exit(1);
    }

    set_init(curr_follow_set);

    while(temp!=NULL){

        if((temp->sym).is_terminal==true){
            set_add_elem(curr_follow_set,(temp->sym).t);
            return curr_follow_set;
        }else{
            //agar non terminal hai phir
            //what else to do with this

            set_union(curr_follow_set,curr_follow_set,first_set[(temp->sym).nt]);

            // if(temp->next==NULL){
            //     return curr_follow_set;
            // }else
            if(temp->next==NULL){
                return curr_follow_set;
            }
            else if(set_find_elem(curr_follow_set,EPSILON)==false){
                return curr_follow_set;
            }else{
                set_remove_elem(curr_follow_set,EPSILON);
                temp=temp->next;
            }

        }

    }

    return curr_follow_set;
}

void populate_first_sets(){


    for(int i=0;i<NUM_OF_RULES;i++){

        int curr_non_terminal=grammar[i].lhs;
        rhsnode* head=grammar[i].head;
        rhsnode* temp=head;


        while(temp!=NULL){

            if((temp->sym).is_terminal==true){
                set_add_elem(first_set[curr_non_terminal],(temp->sym).t);
                break;
            }else{
                //non terminal hai
                ///agar non terminal hai toh aage jaao

                //now what to do
                set_union(first_set[curr_non_terminal],first_set[curr_non_terminal],first_set[(temp->sym).nt]);

                if(set_find_elem(first_set[curr_non_terminal],EPSILON)){

                    if(temp->next!=NULL){
                        set_remove_elem(first_set[curr_non_terminal],EPSILON);
                    }

                    temp=temp->next;
                }

            }

        }

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

                    for(int j=0;j<SET_SIZE;j++){
                        old_follow_set[j]=follow_set[(temp->sym).nt][j];
                    }
// hemant ne glti ki
                    set_union(follow_set[(temp->sym).nt],curr_follow_set,curr_follow_set);

                    for(int j=0;j<SET_SIZE;j++){
                        if(old_follow_set[j]!=curr_follow_set[j]){
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

	for (int j = 0; j < SET_SIZE; j++) {
	  int x = first_set_rhs[j]; // first set

	  while (x) {
		int pos = rightmost_set_bit(&x); // position of terminal in first set
		pos += (INT_NUM_BITS * j);
		if (EPSILON == pos) {
		  for (int j = 0; j < SET_SIZE; j++) {
			int x = follow_set[nt][j]; // if epsilon is there in the first set, add follow
			while (x) {
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

int rightmost_set_bit(int* num){
    int temp1 = (*num & (*num -1)); // Unsets rightmost bit of *num
    int temp2 = temp1 ^ (*num); // temp2 contains only the rightmost set bit of *num
    *num = temp1;
    int lg2 = 0;
    while(temp2){
      lg2++;
      temp2>>=1;
    }
    int position = INT_NUM_BITS - lg2;
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

  while(main_stack->size>0){
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
            break;
          }
        }
        if((stTop->sym).t == tkn.name){
          //increment input pointer
          tkn = getNextToken(source);
          continue;
        }
        else{
          // error
        }
      }
      else{
        //expand using parse table
        int ruleNo = parse_table[(stTop->sym).nt][tkn.name];
        if(ruleNo == NO_MATCHING_RULE){
          //error
        }
        else if(ruleNo == SYN_RULE){
          //error. Enter panic mode
          while(tkn.name!=TK_DOLLAR){
            tkn = getNextToken(source);
            if(set_find_elem(follow_set[(stTop->sym).nt], tkn.name))
              break;
          }
          if(tkn.name == TK_DOLLAR){
            printf("Prasing completed\n");
            break;
          }
        }
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
          while(aux_stack->size>0){
            tree_node* cur = pop(aux_stack);
            push(main_stack, cur);
          }

        }
      }
  }
  return start;
}

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
  printf("%s\n",node);

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

void print_first_set(){


    for(int i=0;i<NUM_OF_NONTERMINALS;i++){

        printf("first_set of (%s)=>{",keyToNT[i]);

        for(int j=0;j<SET_SIZE;j++){

            for(int k=0;k<INT_NUM_BITS;k++){
                if(set_find_elem(first_set[i],j*INT_NUM_BITS+k)==true){
                    printf("%s,",keyToToken[j * INT_NUM_BITS + k]);
                }
            }

            printf("}\n");

        }

    }

}

void print_follow_set(){


    for(int i=0;i<NUM_OF_NONTERMINALS;i++){

        printf("follow_set of (%s)=>{",keyToNT[i]);

        for(int j=0;j<SET_SIZE;j++){

            for(int k=0;k<INT_NUM_BITS;k++){
                if(set_find_elem(follow_set[i],j*INT_NUM_BITS+k)==true){
                    printf("%s,",keyToToken[j * INT_NUM_BITS + k]);
                }
            }

            printf("}\n");

        }

    }

}
