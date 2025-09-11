#include "pipe.h"
#include "shell.h"
#include "commandexec.h"

void handle_pipe(Token *tokens, int numofTokens)
{
    int num_commands = 0;
    int command_starts[buffer];
    int command_lengths[buffer];
    command_starts[0] = 0;
    num_commands = 1;
    int current_token_count = 0;
    for (int i = 0; i < numofTokens; i++)
    {
        if (tokens[i].type == tok_pipe)
        {
            command_lengths[num_commands - 1] = current_token_count;
            current_token_count = 0;
            if (i + 1 < numofTokens)
            {
                command_starts[num_commands] = i + 1;
                num_commands++;
            }
        }
        else
        {
            current_token_count++;
        }
    }
    command_lengths[num_commands - 1] = current_token_count;

    int in_fd = STDIN_FILENO;
    pid_t pids[num_commands];

    for (int i = 0; i < num_commands; i++)
    {
        int pipe_fds[2];

        if (i < num_commands - 1)
        {
            if (pipe(pipe_fds) == -1)
            {
                perror("pipe");
                return;
            }
        }

        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            return;
        }

        if (pids[i] == 0)
        { // Child
            if (i > 0)
            { // Not first command
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (i < num_commands - 1)
            { // Not last command
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[0]);
                close(pipe_fds[1]);
            }
            run_command_internal(tokens + command_starts[i], command_lengths[i]);
        }
        else
        { // Parent
            if (i > 0)
            {
                close(in_fd);
            }
            if (i < num_commands - 1)
            {
                close(pipe_fds[1]);
                in_fd = pipe_fds[0];
            }
        }
    }
    for (int i = 0; i < num_commands; i++)
    {
        wait(NULL);
    }
}