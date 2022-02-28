#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  FILE *fpr = fopen("../grammar.txt", "r");
  FILE *fpw = fopen("../nonTerminals.txt", "w");

  char buf[100];
  char prev[100] = {'n','a'};
  while(fgets(buf,100,fpr)!=NULL){
    buf[99] = '\0';
    char* nt = NULL;
    nt = strtok(buf," ");
    if(strcmp(prev, nt))
      fprintf(fpw,"%s,\n",nt);
    strcpy(prev, nt);
  }
  fclose(fpr);
  fclose(fpw);
  exit(0);
}
