#include "logs.h"
#include "commandexec.h"
#include "parser.h"
#include "shell.h"
#include "hop.h"
#include "reveal.h"

static int read_logs_from_file(char *lines[], int max_lines)
{
    char log_path[PATH_MAX];
    snprintf(log_path, sizeof(log_path), "%s/src/logs.txt", initial_dir);
    FILE *log_file = fopen(log_path, "r");
    if (log_file == NULL)
    {
        // File not existing is not an error, just no logs.
        return 0;
    }

    int line_count = 0;
    char logbuffer[MAX_CMD_LENGTH];
    while (line_count < max_lines && fgets(logbuffer, sizeof(logbuffer), log_file) != NULL)
    {
        logbuffer[strcspn(logbuffer, "\n")] = 0;
        lines[line_count] = strdup(logbuffer);
        if (lines[line_count] == NULL)
        {
            perror("strdup");
            for (int i = 0; i < line_count; i++)
                free(lines[i]);
            fclose(log_file);
            return -1;
        }
        line_count++;
    }
    fclose(log_file);
    return line_count;
}

void print_all_logs()
{
    char log_path[PATH_MAX];
    snprintf(log_path, sizeof(log_path), "%s/src/logs.txt", initial_dir);
    FILE *log_file = fopen(log_path, "r");
    if (log_file == NULL)
    {
        perror("fopen");
        return;
    }
    char logbuffer[MAX_CMD_LENGTH];
    while (fgets(logbuffer, sizeof(logbuffer), log_file) != NULL)
    {
        logbuffer[strcspn(logbuffer, "\n")] = 0;
        printf("%s\n", logbuffer);
    }
    fclose(log_file);
}
void add_log(char *input)
{
    char log_path[PATH_MAX];
    snprintf(log_path, sizeof(log_path), "%s/src/logs.txt", initial_dir);

    char clean_input[MAX_CMD_LENGTH];
    strcpy(clean_input, input);
    char *p = clean_input;
    while (isspace((unsigned char)*p))
    {
        p++;
    }
    if (*p == '\0')
    {
        return;
    }
    char *lines[MAX_LOG_ENTRIES];
    int line_count = read_logs_from_file(lines, MAX_LOG_ENTRIES);
    if (line_count < 0)
    {
        return; // Error occurred
    }
    bool found = false;
    for (int i = 0; i < line_count; i++)
    {
        if (strcmp(lines[i], p) == 0)
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        for (int i = 0; i < line_count; i++)
        {
            free(lines[i]);
        }
        return;
    }
    FILE *log_file = fopen(log_path, "w");
    if (log_file == NULL)
    {
        perror("fopen");
        for (int i = 0; i < line_count; i++)
        {
            free(lines[i]);
        }
        return;
    }
    int start_index = 0;
    if (line_count >= MAX_LOG_ENTRIES)
    {
        start_index = 1;
    }
    for (int i = start_index; i < line_count; i++)
    {
        fprintf(log_file, "%s\n", lines[i]);
    }
    fprintf(log_file, "%s\n", p);
    fclose(log_file);
    for (int i = 0; i < line_count; i++)
    {
        free(lines[i]);
    }
}
void purge_log()
{
    char log_path[PATH_MAX];
    snprintf(log_path, sizeof(log_path), "%s/src/logs.txt", initial_dir);
    FILE *log_file = fopen(log_path, "w");
    if (log_file == NULL)
    {
        perror("fopen");
        return;
    }
    fclose(log_file);
}
void log_exec_index(int index)
{
    char *lines[MAX_LOG_ENTRIES];
    int total_line_count = read_logs_from_file(lines, MAX_LOG_ENTRIES);
    if (total_line_count < 0)
    {
        return; // Error occurred
    }

    if (index <= 0 || index > total_line_count)
    {
        fprintf(stderr, "log: invalid index\n");
        for (int i = 0; i < total_line_count; i++)
            free(lines[i]);
        return;
    }

    Token tokens[buffer];
    int numTokens = tokenise(lines[total_line_count - index], tokens, buffer);
    if (parse_shell_cmd(tokens, numTokens))
    {
        if (strcmp(tokens[0].text, "hop") == 0)
        {
            handle_hop(tokens, numTokens);
        }
        else if (strcmp(tokens[0].text, "reveal") == 0)
        {
            handle_reveal(tokens, numTokens, buffer);
        }
        else
        {
            execute_simple_command(tokens, numTokens);
        }
    }
    for (int i = 0; i < total_line_count; i++)
        free(lines[i]);
}