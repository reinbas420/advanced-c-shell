#ifndef REVEAL_H
#define REVEAL_H
#include "shell.h"
#include "parser.h"
#include <dirent.h>
void handle_reveal(Token *tokens, int numofTokens, size_t buffer_size);
void reveal_only_a(char *path, size_t buffer_size);
void reveal_only_l(char *path, size_t buffer_size);
void reveal_both(char *path, size_t buffer_size);
void reveal_user(char *path, size_t buffer_size);
#endif