#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <linux/limits.h>
#define buffer 500
#define MAX_LOG_ENTRIES 15
#define MAX_CMD_LENGTH 500
extern char cwd[buffer];
extern char prev_dir[buffer];
extern char initial_dir[buffer];
extern int tokenpos;
extern int numTokens;
extern bool hasPipe;
extern bool hasAnd;
extern bool hasSemi;
#endif