#ifndef HOP_H
#define HOP_H
#include "shell.h"
#include "parser.h"
void handle_hop(Token *tokens, int numofTokens);
void home_hop();
void parent_hop();
void prev_hop();
void path_hop(char *arg);
#endif