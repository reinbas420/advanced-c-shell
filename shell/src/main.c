#include "parser.h"
#include "shell.h"
#include "hop.h"
#include "reveal.h"
#include "commandexec.h"
#include "logs.h"
#include "pipe.h"
#include "semi.h"
#include "background.h"

char cwd[buffer];
char prev_dir[buffer];
char initial_dir[buffer];
bool hasPipe = false;
bool hasAnd = false;
bool hasSemi = false;
int main()
{
    prev_dir[0] = '\0';
    if (getcwd(initial_dir, sizeof(initial_dir)) == NULL)
    {
        perror("getcwd() error");
        return 1;
    }
    char input[buffer];
    while (1)
    {
        hasAnd = false;
        hasPipe = false;
        hasSemi = false;
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd() error");
            return 1;
        }
        printf("<mohan@iiit ~%s>:$", cwd);
        if (!fgets(input, sizeof(input), stdin))
        {
            printf("\n"); // Handle Ctrl+D
            break;
        }
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "exit") == 0)
        {
            break;
        }
        Token tokens[buffer];
        int numTokens = tokenise(input, tokens, buffer);
        if (numTokens > 0 && parse_shell_cmd(tokens, numTokens))
        {
            if (strcmp(tokens[0].text, "log") != 0)
            {
                add_log(input);
            }
            if (strcmp(tokens[0].text, "hop") == 0)
            {
                handle_hop(tokens, numTokens);
            }
            else if (strcmp(tokens[0].text, "reveal") == 0)
            {
                handle_reveal(tokens, numTokens, buffer);
            }
            else if (strcmp(tokens[0].text, "log") == 0)
            {
                if (tokens[1].type == tok_end)
                {
                    print_all_logs();
                }
                else if (strcmp(tokens[1].text, "purge") == 0)
                {
                    purge_log();
                }
                else if (strcmp(tokens[1].text, "execute") == 0)
                {
                    log_exec_index(atoi(tokens[2].text));
                }
            }
            else if (hasPipe)
            {
                handle_pipe(tokens, numTokens);
            }
            else if (hasSemi)
            {
                handle_semi(tokens, numTokens);
            }
            else if (hasAnd)
            {
                handle_background(tokens, numTokens);
            }
            else
            {
                execute_simple_command(tokens, numTokens);
            }
        }
    }
    return 0;
}