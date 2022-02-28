int rightmost_set_bit(int* num){
    int temp1 = (*num & (*num -1)); // Unsets rightmost bit of *num
    int temp2 = temp1 ^ (*num); // temp2 contains only the rightmost set bit of *num
    *num = temp1;
    int position = INT_NUM_BITS - 1 - log2(temp2);
    return position;
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

void parser_init(){
    initTable(terminal_table);
    initTable(non_terminal_table);
    for(int i=0;i<NUM_OF_NONTERMINALS;i++){
        set_init(first_set(i));
    }
    for(int i=0;i<NUM_OF_TERMINALS;i++){
        initTable(terminal_table, terminal_string[i], i);
    }
    for(int i=0;i<NUM_OF_NONTERMINALS;i++){
        for(int j=0;j<NUM_OF_TERMINALS;j++){
            parse_table[i][j] = NO_MATCHING_RULE;
        }
    }
    num_tree_nodes = 0;
}

void insert_at_end(rhsnode_ptr* ptr_tail, symbol sym){
    rhsnode_ptr node = (rhsnode_ptr) malloc(sizeof(rhsnode));
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

