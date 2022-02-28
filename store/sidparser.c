void create_parse_table() {
  for (int i = 0; i < NUM_OF_RULES; i++) {
	nonterminal nt = grammar[i].lhs;
	set first_set_rhs = get_rule_first_set(grammar[i].head);

	for (int j = 0; j < SET_SIZE; j++) {
	  ull x = first_set_rhs[j]; // first set

	  while (x) {
		int pos = rightmost_set_bit(&x); // position of terminal in first set
		pos += (ULL_NUM_BITS * j);
		if (EPSILON == pos) {
		  for (int j = 0; j < SET_SIZE; j++) {
			ull x = follow_set[nt][j]; // if epsilon is there in the first set, add follow 
			while (x) {
			  int pos = rightmost_set_bit(&x);
			  pos += (ULL_NUM_BITS * j);	
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


void print_parse_table() {
  for (int i = 0; i < NUM_OF_NONTERMINALS; i++) {
	for (int j = 0; j < NUM_OF_TERMINALS; j++) {
	  if (parse_table[i][j] != NO_MATCHING_RULE) {
		printf("parse_table[%s][%s] : ", non_terminal_string[i], terminal_string[j]);
		print_rule(parse_table[i][j]);
	  }
	}
  }
}