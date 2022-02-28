set get_rule_first_set(rhsnode_ptr head){

    //make the set and do the following things
    rhsnode_ptr temp=head;

    set curr_follow_set=(set)malloc(sizeof(ull)*SET_SIZE);

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


void populate_first_set(){


    for(int i=0;i<NUM_OF_RULES;i++){

        int curr_non_terminal=grammer[i].lhs;
        rhsnode_ptr head=grammer[i].head;
        rhsnode_ptr temp=head;


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


void populate_follow_set(){


    bool has_change=true;

    while(has_change==true){

        has_change=false;
        for(int i=0;i<NUM_OF_RULES;i++){

            int lhs=grammer[i].lhs;
            rhsnode_ptr head=grammer[i].head;
            rhsnode_ptr temp=head;

            while(temp!=NULL){

                if((temp->sym).is_terminal==true){
                    temp=temp->next;
                }else{
                    //agar koi non terminal hai toh phir uska follow_set calculate kar do

                    set old_follow_set=(set)malloc(sizeof(ull)*SET_SIZE);
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

                    set_union(follow_set[(temp->next).nt],curr_follow_set,curr_follow_set);

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