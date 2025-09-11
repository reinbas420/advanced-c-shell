#include"background.h"
#include"parser.h"

void handle_background(Token *tokens,int numofTokens){
    if (numofTokens == 0)
    {
        return; // Nothing to execute
    }
    pid_t pid = fork();
    printf("%d\n",pid);
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    { // Child process
        run_command_internal(tokens, numTokens);
    }
    else
    { // Parent process
        return;
    }
}