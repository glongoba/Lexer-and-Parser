#ifndef GSH_H
#define GSH_H

typedef enum {
    TOKEN_WORD, //command
    TOKEN_PIPE, // |
    TOKEN_REDIRIN, // <
    TOKEN_REDIROUT, // >
    TOKEN_BACKGROUND, // &
    TOKEN_APPEND, // >>
    TOKEN_EOF // End of the line
} ttype;

typedef struct{
    ttype type;
    char* line;
} listToken;

char *readGSH(void);
listToken createTokenGSH(ttype typez,const char* linez,int size);
listToken* tokenizeGSH(char *line,int *tokenNumber);
void freelistTokenGSH(listToken *list,int count);


#endif
