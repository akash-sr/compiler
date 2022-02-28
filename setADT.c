#include "headerFiles/setADT.h"

void set_init(set s){
    for(int i=0;i<SET_SIZE;i++){
        s[i] = 0;
    }
}

bool set_find_elem(set s, int num){
    int set_number = num / INT_NUM_BITS;
    int position = num % INT_NUM_BITS;
    if(s[set_number] & (1 << (INT_NUM_BITS - position - 1))){
        return true;
    }
    return false;
}

void set_add_elem(set s, int num){
    int set_number = num / INT_NUM_BITS;
    int position = num % INT_NUM_BITS;
    s[set_number] |= (1 << (INT_NUM_BITS - position - 1));
}

void set_remove_elem(set s, int num){
    int set_number = num / INT_NUM_BITS;
    int position = num % INT_NUM_BITS;
    s[set_number] &= (~(1 << (INT_NUM_BITS - position - 1)));
}

void set_union(set dest, set first, set second){
    for(int i=0;i<SET_SIZE;i++){
        dest[i] = first[i] | second[i];
    }
}

void set_intersection(set dest, set first, set second){
    for(int i=0;i<SET_SIZE;i++){
        dest[i] = first[i] & second[i];
    }
}

bool is_superset(set first, set second){
    for(int i=0;i<SET_SIZE;i++){
        if(!((first[i] & second[i]) ^ (second[i]))){
            return false;
        }
    }
    return true;
}
