#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "lexer/lexer.h"


#define sizeArgs 5
pipeline* parseGSH(listToken* lt,int numToken){

    pipeline *p=malloc(sizeof(pipeline));
    if(p==NULL){
        perror("gsh: allocation error. ");
        exit(EXIT_FAILURE);
    }
    p->numbComandi=0;
    p->comm=NULL;
    p->background=0;


    if(lt == NULL){
        perror("gsh: token error. ");
        exit(EXIT_FAILURE);
    }

    // Early syntax error validation
    if(lt[0].type==TOKEN_REDIRIN || lt[0].type==TOKEN_REDIROUT || lt[0].type==TOKEN_BACKGROUND || lt[0].type==TOKEN_PIPE || lt[0].type==TOKEN_APPEND){
        perror("gsh: syntax error. ");
        exit(EXIT_FAILURE);
    }

    int numCommand=5;
    command *cmd=malloc(numCommand*sizeof(command));

    if(cmd == NULL){
        perror("gsh: parsing error. ");
        exit(EXIT_FAILURE);
    }

    int numArgs=5;
    int indCommand=0;
    int indArgs=0;
    int indToken=0;
    cmd[indCommand].args=malloc(numArgs*sizeof(char*));
    if(cmd[indCommand].args == NULL){
        perror("gsh: command error. ");
        exit(EXIT_FAILURE);
    }
    cmd[indCommand].inputFile=NULL;
    cmd[indCommand].outputFile=NULL;
    cmd[indCommand].appendMode=0;
    cmd[indCommand].args[indArgs]=lt[indToken].line;
    indArgs++;
    indToken++;

    while(indToken<numToken && lt[indToken].type!=TOKEN_EOF){

        if(indCommand>=numCommand){
            numCommand=numCommand*2;
            command *temp=realloc(cmd,numCommand*sizeof(command));
            if(temp==NULL){
                perror("gsh: memory leak error. ");
                exit(EXIT_FAILURE);
            }
            else{
                cmd=temp;
            }
        }
        if(indArgs>=numArgs){
            numArgs=numArgs*2;
            cmd[indCommand].args=realloc(cmd[indCommand].args,numArgs*sizeof(char*));
        }

        if(lt[indToken].type==TOKEN_WORD){
            cmd[indCommand].args[indArgs]=lt[indToken].line;
            indArgs++;
            indToken++;
        }
        else if(lt[indToken].type==TOKEN_REDIROUT){
            if(indToken+1 >= numToken || lt[indToken + 1].type!=TOKEN_WORD){
            fprintf(stderr,"gsh: syntax error. \n");
            return NULL;}
            else{
                if(cmd[indCommand].outputFile==NULL){
                cmd[indCommand].outputFile=strdup(lt[indToken + 1].line);
                }
                else{
                    free(cmd[indCommand].outputFile);
                    cmd[indCommand].outputFile=strdup(lt[indToken+ 1].line);
                }
                indToken+=2;
            }
        }
        else if(lt[indToken].type==TOKEN_REDIRIN){
            if(indToken+1 >= numToken || lt[indToken+ 1].type!=TOKEN_WORD){
            fprintf(stderr,"gsh: syntax error. \n");
            return NULL;}
            if(cmd[indCommand].inputFile!=NULL) free(cmd[indCommand].inputFile);
            cmd[indCommand].inputFile=strdup(lt[indToken+1].line);
            indToken+=2;
        }
        else if(lt[indToken].type==TOKEN_APPEND){
            if(indToken+1 >= numToken || lt[indToken+1].type!=TOKEN_WORD){
            fprintf(stderr,"gsh: syntax error. \n");
            return NULL;}
            else{
                if(cmd[indCommand].outputFile!=NULL) free(cmd[indCommand].outputFile);
                cmd[indCommand].outputFile=strdup(lt[indToken+1].line);
                indToken+=2;
                cmd[indCommand].appendMode=1;
            }
        }
        else if(lt[indToken].type==TOKEN_PIPE){
            if(lt[indToken+1].type==TOKEN_PIPE || lt[indToken+1].type==TOKEN_REDIRIN || lt[indToken+1].type==TOKEN_REDIROUT || lt[indToken+1].type==TOKEN_APPEND || lt[indToken+1].type==TOKEN_EOF){
                fprintf(stderr,"gsh: syntax error. \n");
                return NULL;
            }
            else{
                cmd[indCommand].args[indArgs]=NULL;
                indCommand++;
                indArgs=0;
                cmd[indCommand].args=malloc(sizeArgs*sizeof(char*));
                if(cmd[indCommand].args==NULL){
                    perror("gsh: allocation error. ");
                    exit(EXIT_FAILURE);
                }
                cmd[indCommand].inputFile=NULL;
                cmd[indCommand].outputFile=NULL;
                cmd[indCommand].appendMode=0;
            }
            indToken++;
        }
        else if(lt[indToken].type==TOKEN_BACKGROUND){
            if(lt[indToken+1].type!=TOKEN_EOF){
                perror("gsh: syntax error. ");
                exit(EXIT_FAILURE);
            }
            else p->background=1;

            indToken++;
        }
    }

    cmd[indCommand].args[indArgs]=NULL;
    p->comm=cmd;
    p->numbComandi=indCommand+1;

    return p;
}

void freePipelineGSH(pipeline *p){
    if(p==NULL) return;
    int i=0;
    while(i<p->numbComandi){
        if(p->comm[i].args!=NULL)
        free(p->comm[i].args);
        if(p->comm[i].inputFile!=NULL)
        free(p->comm[i].inputFile);
        if(p->comm[i].outputFile!=NULL)
        free(p->comm[i].outputFile);
        i++;
    }
    free(p->comm);
    free(p);
}