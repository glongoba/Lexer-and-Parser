#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"

char *readGSH(void){
    
    char *line=NULL;
    size_t bufsize=0;

    if(getline(&line,&bufsize,stdin)==-1){ 
        if(feof(stdin)) exit(EXIT_SUCCESS);      
        else{                                        
            perror("gsh: Errore nella lettura. ");
            exit(EXIT_FAILURE);
        }
    }

    // in getline if there is an errore, and it isn't possible to read
    // it is crucial to recognise if ( feof(stdin) ) the user
    // clicked send as en error, so everything was alright
    // or it was a problem -> in that case perror

    return line;

}

listToken createTokenGSH(ttype typez,const char* linez,int size){
    listToken lt;
    lt.type=typez;
    lt.line=malloc((size+1)*sizeof(char));
    if(lt.line == NULL){
        perror("gsh: token management error.");
        exit(EXIT_FAILURE);
    }
    strncpy(lt.line,linez,size);
    lt.line[size]='\0';
    return lt;
}

listToken* tokenizeGSH(char *line,int *tokenNumber){
    size_t capacity=8;
    listToken *lt=malloc(capacity*sizeof(listToken));

    if(lt == NULL){
        perror("gsh: token management error. ");
        exit(EXIT_FAILURE);
    }

    size_t i=0;
    size_t countStruct=0;
    size_t len=strlen(line);
    while(i<len){
        if(line[i]==' ' || line[i]=='\t'|| line[i]=='\n'){
            i++;
            continue;
        }
        
        if(countStruct>=capacity){
            capacity= capacity*2;
            lt=realloc(lt,capacity*sizeof(listToken));
            if(lt == NULL){
                perror("gsh: token management error.");
                exit(EXIT_FAILURE);
            }
        }

        if(line[i]=='|'){
            lt[countStruct]=createTokenGSH(TOKEN_PIPE,"|",1);
            countStruct++;
            i++;
            }
        else if(line[i]=='<'){
            lt[countStruct]=createTokenGSH(TOKEN_REDIRIN,"<",1);
            countStruct++;
            i++;
        }
        else if(line[i]=='>'){
            if(line[i+1]=='>'){
                lt[countStruct]=createTokenGSH(TOKEN_APPEND,">>",2);
                countStruct++;
                i+=2;
            }
            else{
                lt[countStruct]=createTokenGSH(TOKEN_REDIROUT,">",1);
                countStruct++;
                i++;
            }
        }
        else if(line[i]=='&'){
            lt[countStruct]=createTokenGSH(TOKEN_BACKGROUND,"&",1);
            countStruct++;
            i++;
        }
        else{
            size_t start=i;
            while(i<len && line[i]!=' ' && line[i]!='\t' && line[i]!='>' && line[i]!='<' && line[i]!='&' && line[i]!='|'){
                if(line[i]=='"' || line[i]=='\''){
                i++;
                while(i<len && line[i]!='"' && line[i]!='\'') i++;
                if(i<len) i++;}
                else i++;
            }
            size_t wlen=i-start;
            lt[countStruct]=createTokenGSH(TOKEN_WORD,&line[start],wlen);
            countStruct++;
        }
        
    }
    if(countStruct>=capacity){
            capacity= capacity*2;
            lt=realloc(lt,capacity*sizeof(listToken));
            if(lt == NULL){
                perror("gsh: token management error.");
                exit(EXIT_FAILURE);
            }
        }
    lt[countStruct]=createTokenGSH(TOKEN_EOF,"",0);
        *tokenNumber=countStruct+1;
        free(line);
        return lt;
}

void freelistTokenGSH(listToken *list,int count){
    if(list==NULL) return;
    for(int i=0;i<count;i++){
        free(list[i].line);
    }
    free(list);
}