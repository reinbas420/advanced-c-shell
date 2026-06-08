#include "hop.h"

void home_hop()
{
    char *home = getenv("HOME");
    if (home == NULL)
    {
        fprintf(stderr, "hop: HOME not set\n");
        return;
    }
    char current_dir[buffer];
    if (getcwd(current_dir, buffer) == NULL)
    {
        perror("hop: getcwd");
        return;
    }
    if (chdir(home) != 0)
    {
        fprintf(stderr, "No such directory!\n");
        return;
    }
    strcpy(prev_dir, current_dir);
    if (getcwd(cwd, buffer) == NULL)
    {
        perror("hop: getcwd");
    }
}
void parent_hop()
{
    char current_dir[buffer];
    if (getcwd(current_dir, buffer) == NULL)
    {
        perror("hop: getcwd");
        return;
    }

    if (chdir("..") != 0)
    {
        fprintf(stderr, "No such directory!\n");
        return;
    }
    strcpy(prev_dir, current_dir);
    if (getcwd(cwd, buffer) == NULL)
    {
        perror("hop: getcwd");
    }
}
void prev_hop()
{
    if (prev_dir[0] == '\0')
    {
        fprintf(stderr, "hop: OLDPWD not set\n");
        return;
    }

    char current_dir[buffer];
    if (getcwd(current_dir, buffer) == NULL)
    {
        perror("hop: getcwd");
        return;
    }

    if (chdir(prev_dir) != 0)
    {
        fprintf(stderr, "No such directory!\n");
        return;
    }

    strcpy(prev_dir, current_dir);
    if (getcwd(cwd, buffer) == NULL)
    {
        perror("hop: getcwd");
    }
}
void path_hop(char *arg)
{
    char current_dir[buffer];
    if (getcwd(current_dir, buffer) == NULL)
    {
        perror("hop: getcwd");
        return;
    }

    if (chdir(arg) != 0)
    {
        fprintf(stderr, "No such directory!\n");
        return;
    }

    strcpy(prev_dir, current_dir);
    if (getcwd(cwd, buffer) == NULL)
    {
        perror("hop: getcwd");
    }
}
void handle_hop(Token *tokens, int numofTokens)
{
    if (numofTokens == 1)
    {
        home_hop();
        return;
    }
    for (int i = 1; i < numofTokens; i++)
    {
        char *arg = tokens[i].text;
        if (strcmp(arg, "~") == 0)
        {
            home_hop();
        }
        else if (strcmp(arg, "..") == 0)
        {
            parent_hop();
        }
        else if ((strcmp(arg, "-") == 0))
        {
            prev_hop();
        }
        else if (strcmp(arg, ".") == 0)
        {
            strcpy(prev_dir, cwd);
            if (getcwd(cwd, buffer) == NULL)
            {
                perror("hop: getcwd");
            }
            continue;
        }
        else
        {
            path_hop(arg);
        }
    }
}