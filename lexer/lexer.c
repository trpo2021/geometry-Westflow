#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

TokenList* lex(const char* data, size_t length, int line)
{
    LexerState ls = LexerDefault;
    TokenList* tokens = NULL;
    tokens = token_list_init(tokens);
    char* token_content = "";
    bool has_error = false;
    for (int column = 0; column < length; column++)
    {
        char c = data[column];
        switch (ls)
        {
        case LexerDefault:
            if (isalpha(c))
            {
                token_content = append_char(token_content, c);
                ls = LexerWord;
                if (column + 1 >= length)
                {
                    tokens = token_list_add(
                        tokens, create_token_from_string(
                                    token_content, TokenWord, line,
                                    column + 1 - strlen(token_content)));
                    free(token_content);
                }
            }
            else if (isdigit(c) || c == '-')
            {
                token_content = append_char(token_content, c);
                ls = LexerNumber;
                if (column + 1 >= length)
                {
                    tokens = token_list_add(
                        tokens, create_token_from_string(
                                    token_content, TokenNumber, line,
                                    column + 1 - strlen(token_content)));
                    free(token_content);
                }
            }
            else if (c == TokenComma || c == TokenOpBracket ||
                     c == TokenClBracket)
            {
                tokens = token_list_add(
                    tokens, create_token_from_char(c, line, column));
            }
            else if (c != ' ')
            {
                print_exception(data, column);
                printf("Unexpected token %c at line %d and column %d\n\n", c,
                       line, column);
                has_error = true;
            }
            break;
        case LexerWord:
            if (isalpha(c))
            {
                token_content = append_char(token_content, c);
                if (column + 1 >= length)
                {
                    tokens = token_list_add(
                        tokens, create_token_from_string(
                                    token_content, TokenWord, line,
                                    column + 1 - strlen(token_content)));
                    free(token_content);
                    ls = LexerDefault;
                }
            }
            else
            {
                tokens = token_list_add(
                    tokens,
                    create_token_from_string(token_content, TokenWord, line,
                                             column - strlen(token_content)));
                free(token_content);
                token_content = "";
                --column;
                ls = LexerDefault;
            }
            break;
        case LexerNumber:
            if (c == '.' || isdigit(c))
            {
                token_content = append_char(token_content, c);
                if (column + 1 >= length)
                {
                    tokens = token_list_add(
                        tokens, create_token_from_string(
                                    token_content, TokenNumber, line,
                                    column + 1 - strlen(token_content)));
                    free(token_content);
                    ls = LexerDefault;
                }
            }
            else
            {
                tokens = token_list_add(
                    tokens,
                    create_token_from_string(token_content, TokenNumber, line,
                                             column - strlen(token_content)));
                free(token_content);
                token_content = "";
                --column;
                ls = LexerDefault;
            }
            break;
        }
    }
    if (has_error)
    {
        return NULL;
    }
    return tokens;
}