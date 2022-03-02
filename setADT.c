#include "headerFiles/setADT.h"

void set_init(set s){
    for(int i=0;i<SET_SIZE;i++){
        s[i] = false;
    }
}

bool set_find_elem(set s, int num){
    return s[num] == true;
}

void set_add_elem(set s, int num){
    s[num] = true;
}

void set_remove_elem(set s, int num){
    s[num] = false;
}

void set_union(set dest, set first, set second){
    for(int i=0;i<SET_SIZE;i++){
        dest[i] = first[i] || second[i];
    }
}

void set_intersection(set dest, set first, set second){
    for(int i=0;i<SET_SIZE;i++){
        dest[i] = first[i] && second[i];
    }
}

bool is_superset(set first, set second){
    for(int i=0;i<SET_SIZE;i++){
        if(second == true && first == false){
            return false;
        }
    }
    return true;
}
