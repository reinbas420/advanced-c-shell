#include"semi.h"
#include "parser.h"
#include "commandexec.h"

void handle_semi(Token *tokens, int numofTokens){
    int startTok = 0;
    for (int i = 0; i < numofTokens; i++) {
        if (tokens[i].type == tok_semi) {
            int command_len = i - startTok;
            if (command_len > 0) {
                execute_simple_command(tokens + startTok, command_len);
            }
            startTok = i + 1;
        }
    }
    if (startTok < numofTokens) {
        int command_len = numofTokens - startTok;
        if (command_len > 0) {
            execute_simple_command(tokens + startTok, command_len);
        }
    }
}

