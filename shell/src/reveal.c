#include "reveal.h"
int compare_file_names(const void *a, const void *b)
{
    const char *name1 = *(const char **)a;
    const char *name2 = *(const char **)b;
    return strcmp(name1, name2); // ASCII order
}
void reveal_user(char *path, size_t buffer_size)
{
    DIR *dir;
    struct dirent *files;
    dir = opendir(path);
    if (dir == NULL)
    {
        perror("reveal: opendir");
        return;
    }
    char *file_names[buffer_size];
    int count = 0;
    while ((files = readdir(dir)) != NULL)
    {
        if (files->d_name[0] != '.')
        {
            file_names[count++] = strdup(files->d_name);
        }
    }
    closedir(dir);
    qsort(file_names, count, sizeof(char *), compare_file_names);
    for (int i = 0; i < count; i++)
    {
        printf("%s\t", file_names[i]);
    }
    for (int i = 0; i < count; i++)
    {
        free(file_names[i]);
    }
    printf("\n");
}
void reveal_only_a(char *path, size_t buffer_size)
{
    DIR *dir;
    struct dirent *files;
    dir = opendir(path);
    if (dir == NULL)
    {
        perror("reveal: opendir");
        return;
    }
    char *file_names[buffer_size];
    int count = 0;
    while ((files = readdir(dir)) != NULL)
    {
        file_names[count++] = strdup(files->d_name);
    }
    closedir(dir);
    qsort(file_names, count, sizeof(char *), compare_file_names);
    for (int i = 0; i < count; i++)
    {
        printf("%s\t", file_names[i]);
    }
    for (int i = 0; i < count; i++)
    {
        free(file_names[i]);
    }
    printf("\n");
}
void reveal_only_l(char *path, size_t buffer_size)
{
    DIR *dir;
    struct dirent *files;
    dir = opendir(path);
    if (dir == NULL)
    {
        perror("reveal: opendir");
        return;
    }
    char *file_names[buffer_size];
    int count = 0;
    while ((files = readdir(dir)) != NULL)
    {
        file_names[count++] = strdup(files->d_name);
    }
    closedir(dir);
    qsort(file_names, count, sizeof(char *), compare_file_names);
    for (int i = 0; i < count; i++)
    {
        if (file_names[i][0] != '.')
        {
            printf("%s\n", file_names[i]);
        }
    }
    for (int i = 0; i < count; i++)
    {
        free(file_names[i]);
    }
}
void reveal_both(char *path, size_t buffer_size)
{
    DIR *dir;
    struct dirent *files;
    dir = opendir(path);
    if (dir == NULL)
    {
        perror("reveal: opendir");
        return;
    }
    char *file_names[buffer_size];
    int count = 0;
    while ((files = readdir(dir)) != NULL)
    {
        file_names[count++] = strdup(files->d_name);
    }
    closedir(dir);
    qsort(file_names, count, sizeof(char *), compare_file_names);
    for (int i = 0; i < count; i++)
    {
        printf("%s\n", file_names[i]);
    }
    for (int i = 0; i < count; i++)
    {
        free(file_names[i]);
    }
}
void handle_reveal(Token *tokens, int numofTokens, size_t buffer_size)
{
    bool hasL = false;
    bool hasA = false;
    char *target = NULL;
    for (int i = 1; i < numofTokens; i++)
    {
        char *arg = tokens[i].text;
        if(arg[0]=='~')break;
        if (arg[0] == '-' && strlen(arg) > 1)
        {
            for (size_t j = 1; j < strlen(arg); j++)
            {
                if (arg[j] == 'l')
                {
                    hasL = true;
                }
                else if (arg[j] == 'a')
                {
                    hasA = true;
                }
            }
        }
        else if (target != NULL)
        {
            fprintf(stderr, "reveal: Too many arguments\n");
            return;
        }
        else
        {
            target = arg;
        }
    }
    char *path_to_reveal = target ? target : cwd;
    if (hasA && hasL)
    {
        reveal_both(path_to_reveal, buffer_size);
    }
    else if (hasL)
    {
        reveal_only_l(path_to_reveal, buffer_size);
    }
    else if (hasA)
    {
        reveal_only_a(path_to_reveal, buffer_size);
    }
    else
    {
        reveal_user(path_to_reveal, buffer_size);
    }
}