#ifndef COMMANDEEXEC_H
#define COMMANDEEXEC_H
#include "shell.h"
#include "reveal.h"
#include <fcntl.h>
void execute_simple_command(Token *tokens, int numTokens);
void run_command_internal(Token *tokens, int numTokens);
#endif