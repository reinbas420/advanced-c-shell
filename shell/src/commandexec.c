#include "commandexec.h"
#define buffer 500


void execute_simple_command(Token *tokens, int numTokens)
{
    if (numTokens == 0)
    {
        return; // Nothing to execute
    }
    pid_t pid = fork();
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
        int status;
        waitpid(pid, &status, 0);
    }
}

void run_command_internal(Token *tokens, int numTokens)
{
    char *argv[buffer + 1];
    int argc = 0;
    char *inputFile = NULL;
    char *outputFile = NULL;
    bool append_output = false;
    int i = 0;
    while (i < numTokens)
    {
        TokenType type = tokens[i].type;
        if (type == tok_name)
        {
            argv[argc++] = tokens[i].text;
        }
        else if (type == tok_lt)
        {
            i++;
            if (i < numTokens && tokens[i].type == tok_name)
            {
                inputFile = tokens[i].text;
            }
            else
            {
                fprintf(stderr, "syntax error near unexpected token `newline'\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (type == tok_gt)
        {
            i++;
            if (i < numTokens && tokens[i].type == tok_name)
            {
                outputFile = tokens[i].text;
                append_output = false;
            }
            else
            {
                fprintf(stderr, "syntax error near unexpected token `newline'\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (type == tok_gt2)
        {
            i++;
            if (i < numTokens && tokens[i].type == tok_name)
            {
                outputFile = tokens[i].text;
                append_output = true;
            }
            else
            {
                fprintf(stderr, "syntax error near unexpected token `newline'\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            break;
        }
        i++;
    }
    argv[argc] = NULL;

    if (inputFile)
    {
        int fd_in = open(inputFile, O_RDONLY);
        if (fd_in == -1)
        {
            perror("open input file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    if (outputFile)
    {
        int flags = O_WRONLY | O_CREAT;
        if (append_output)
        {
            flags |= O_APPEND;
        }
        else
        {
            flags |= O_TRUNC;
        }
        int fd_out = open(outputFile, flags, 0644);
        if (fd_out == -1)
        {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }

    if (argc > 0)
    {
        execvp(argv[0], argv);
        perror(argv[0]); // execvp only returns on error
        exit(EXIT_FAILURE);
    }
    else
    {
        exit(EXIT_SUCCESS); // For commands with only redirections
    }
}
