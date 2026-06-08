#include "parser.h"
#include "shell.h"
#define buffer 500
int tokenpos = 0;
int numTokens = 0;
int is_operator(const char *s, int *len, TokenType *type)
{
    if (strncmp(s, ">>", 2) == 0)
    {
        *len = 2, *type = tok_gt2;
        return 1;
    }
    if (*s == '&')
    {
        hasAnd=true;
        *len = 1, *type = tok_and;
        return 1;
    }
    if (*s == '|')
    {
        hasPipe=true;
        *len = 1;
        *type = tok_pipe;
        return 1;
    }
    if (*s == '>')
    {
        *len = 1;
        *type = tok_gt;
        return 1;
    }
    if (*s == '<')
    {
        *len = 1;
        *type = tok_lt;
        return 1;
    }
    if (*s == ';')
    {
        hasSemi=true;
        *len = 1;
        *type = tok_semi;
        return 1;
    }
    return 0;
}

int tokenise(const char *input, Token *tokens, int maxTokens)
{
    int i = 0, pos = 0; //
    while (input[pos] && i < maxTokens)
    {
        while (isspace(input[pos]))
            pos++;
        if (!input[pos])
            break;

        int oplen;
        TokenType type;
        if (is_operator(&input[pos], &oplen, &type))
        {
            tokens[i].type = type;
            strncpy(tokens[i].text, &input[pos], oplen);
            tokens[i].text[oplen] = '\0';
            pos += oplen;
            i++;
            continue; // found as an operator so no need for below code
        }
        // Finding words
        int start = pos;
        while (input[pos] && !isspace(input[pos]) && !is_operator(&input[pos], &oplen, &type))
            pos++;
        int len = pos - start;
        if (len > 0)
        {
            tokens[i].type = tok_name;
            strncpy(tokens[i].text, &input[start], len);
            tokens[i].text[len] = '\0';
            i++;
        }
    }
    tokens[i].type = tok_end;
    tokens[i].text[0] = '\0';
    return i;
}

// llm
bool parse_shell_cmd(Token *tokens,int numofTokens)
{
    tokenpos = 0;
    numTokens =numofTokens;
    if (tokens[tokenpos].type == tok_end)
    {
        return true; // Handle empty input gracefully
    }

    // Parse: cmd_group ((& | ;) cmd_group)*
    if(!parse_cmd_group(tokens)) return false;
    while (tokenpos < numTokens && (tokens[tokenpos].type == tok_semi || tokens[tokenpos].type == tok_and))
    {
        if (tokens[tokenpos].type == tok_and && (tokenpos + 1 >= numTokens)) {
            break;
        }

        tokenpos++; // Consume separator

        if (tokenpos >= numTokens)
        {
            printf("Invalid Syntax!\n");
            return false;
        }
        if(!parse_cmd_group(tokens)) return false; // next cmd
    }

    if (tokenpos < numTokens && tokens[tokenpos].type == tok_and) {
        tokenpos++;
    }
    if (tokenpos != numTokens) {
        printf("Invalid Syntax!\n");
        return false;
    }
    return true;
}
// llm
bool parse_cmd_group(Token *tokens)
{
    if(!parse_atomic(tokens))return false;
    while (tokenpos < numTokens && tokens[tokenpos].type == tok_pipe)
    {
        tokenpos++;
        if (tokenpos >= numTokens || tokens[tokenpos].type == tok_end)
        {
            printf("Invalid Syntax!\n");
            return false;
        }
        if(!parse_atomic(tokens))return false;
    }
    return true;
}
bool parse_atomic(Token *tokens)
{
    if (tokenpos < numTokens && tokens[tokenpos].type == tok_name)
    {
        tokenpos++;
        while (tokenpos < numTokens)
        {
            if (tokens[tokenpos].type == tok_name)
            {
                tokenpos++; // Consume argument
            }
            else if (tokens[tokenpos].type == tok_lt ||
                     tokens[tokenpos].type == tok_gt ||
                     tokens[tokenpos].type == tok_gt2)
            {
                tokenpos++;
                if(!parse_name(tokens))return false;
            }
            else
            {
                break; // No more arguments or redirections
            }
        }
    }
    else
    {
        printf("Invalid Syntax!\n");
        return false;
    }
    return true;
}
bool parse_name(Token *tokens)
{
    if (tokenpos < numTokens && tokens[tokenpos].type == tok_name)
    {
        tokenpos++;
        return true;
    }
    else
    {
        printf("Invalid Syntax!\n");
        return false;
    }
    return true;
}
