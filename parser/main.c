#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "parser.h"

int main(void){
    int num=0;
    printf(" Inserisci testo: \n");
    char *line=readGSH();
    listToken *lt=tokenizeGSH(line,&num);
    printf(" NUM: %d \n",num);
    pipeline *p=parseGSH(lt,num);
    if(p==NULL){
        freelistTokenGSH(lt,num);
        return -1;
    }
    int i=0;
    while(i < p->numbComandi){
        int j=0;
        while(p->comm[i].args[j]!=NULL){
            printf(" Comando %d: %s \n",i+1,p->comm[i].args[j]);
            j++;
        }
        if(p->comm[i].inputFile!=NULL){
            printf(" Comando %d input file: %s \n",i+1,p->comm[i].inputFile);
        }
        if(p->comm[i].outputFile!=NULL){
            printf(" Comando %d output file: %s \n",i+1,p->comm[i].outputFile);
        }
        i++;
    }
    freelistTokenGSH(lt,num);
    freePipelineGSH(p);
    return 0;
}