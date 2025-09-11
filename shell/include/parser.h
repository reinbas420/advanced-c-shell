#ifndef PARSER_H
#define PARSER_H
#include"shell.h"
#include<stdio.h>
#include<regex.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<limits.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/wait.h> 
typedef enum {
    tok_name, tok_pipe, tok_and, tok_semi,
    tok_gt, tok_gt2,
    tok_lt,
    tok_end,tok_notvalid
} TokenType;

typedef struct {
    TokenType type;
    char text[128];
} Token;

int is_operator(const char*s,int*len,TokenType *type);
int tokenise(const char*input,Token*tokens,int maxTokens);
bool parse_shell_cmd(Token *tokens,int numofTokens);  
bool parse_cmd_group(Token*tokens);  
bool parse_atomic(Token*tokens); 
bool parse_name(Token*tokens);  
#endif