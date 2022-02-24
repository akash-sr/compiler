#include<stdio.h>
#include<stdlib.h>



int buffer_length=50;
int fwd_ptr=50;
int retract_count=0;
char buffer[50];


void retract(int num_of_char) {
  fwd_ptr -= num_of_char;
  if (fwd_ptr < 0) {
    fwd_ptr += buffer_length;
  }
  retract_count += num_of_char;
}

void fill_buffer(FILE* fp) //this is the function which fills the buffer when needed
{
    int num;
    if (retract_count !=0){
        return;
    }
    if (fwd_ptr == buffer_length)
    {
        fwd_ptr=0;
        // rounds_completed++;
    }
    // num stores the numbmer of characters which are read.
    // the characters which are read are stored in an array of characters named buffer. starting from index buffer/2 or from index 0
    num = fread(&buffer[fwd_ptr],1,buffer_length/2,fp);
    if(num == buffer_length/2)
    {
        return;
    }
    buffer[num + fwd_ptr] = EOF; // ?????????? how come he is treating EOF as a character in C????????
}


char get_char(FILE* fp)
{
    if(fwd_ptr==buffer_length||fwd_ptr==buffer_length/2)
    {
        fill_buffer(fp);
    }
    char c = buffer[fwd_ptr];
    // int lex_index = (fwd_ptr -lexeme_begin >= 0)?(fwd_ptr-lexeme_begin):(fwd_ptr-lexeme_begin+buffer_length);
    // if (lex_index<MAX_LEXEME_LEN)
    // {
    //     lexeme[lex_index] =c;
    // }
    fwd_ptr+=1;
    if (retract_count > 0)
        retract_count--;
    return c;
}


void remove_comments(char * sourceFile){



    FILE * fptr=fopen(sourceFile,"r");
    FILE * cptr=fopen("wcomment.txt","w");

    if(fptr==NULL || cptr==NULL){
        printf("Files can't be opened try again");
        exit(1);
    }else{
        printf("Files Opened\n");
    }


    printf("%d\n",1);

    // char buff[255];
    char c= get_char(fptr);   //fgetc ke badle get_char daalna padega kyoki waha hum chahenge ki buffer me pehle read kar le phir hum kare
    while (c!= EOF)
    {

        if(c=='%'){

            while((c!='\n') && (c != EOF)){
                c=get_char(fptr);
            }

        }else{
            fputc(c,cptr);
        }

        c=get_char(fptr);

    }

    fclose(fptr);
    fclose(cptr);

}


int main(){


    char sourceFile[20];

    scanf("%s",sourceFile);

    // printf("%s\n",sourceFile);

    remove_comments(sourceFile);

    printf("%s\n",sourceFile);

    return 0;
}
