#ifndef PARSER_C
#define PARSER_C

#include "lexer/lexer.h"

typedef struct{
    char **args; // a single command
    char *inputFile; // input file
    char *outputFile; // output file
    int appendMode; // append? (>>)
} command;

typedef struct{
    command *comm; 
    int numbComandi;
    int background; // background? (&)
} pipeline; // organization of all the line

pipeline* parseGSH(listToken* lt,int numToken);
void freePipelineGSH(pipeline *p);

#endif
