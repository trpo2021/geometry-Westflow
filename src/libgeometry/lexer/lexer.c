#include <ctype.h>
#include <libgeometry/lexer/lexer.h>
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
                    int t_column = column + 1 - strlen(token_content);
                    Token item = create_token_from_string(token_content, TokenWord, line, t_column);
                    tokens = token_list_add(tokens, item);
                    free(token_content);
                }
            }
            else if (isdigit(c) || c == '-')
            {
                token_content = append_char(token_content, c);
                ls = LexerNumber;
                if (column + 1 >= length)
                {
                    int t_column = column + 1 - strlen(token_content);
                    Token item = create_token_from_string(token_content, TokenNumber, line, t_column);
                    tokens = token_list_add(tokens, item);
                    free(token_content);
                }
            }
            else if (c == TokenComma || c == TokenOpBracket || c == TokenClBracket)
            {
                tokens = token_list_add(tokens, create_token_from_char(c, line, column));
            }
            else if (c != ' ')
            {
                print_exception(data, column);
                unknown_symbol_exception(c, line, column);
                has_error = true;
            }
            break;
        case LexerWord:
            if (isalpha(c))
            {
                token_content = append_char(token_content, c);
                if (column + 1 >= length)
                {
                    int t_column = column + 1 - strlen(token_content);
                    Token item = create_token_from_string(token_content, TokenWord, line, t_column);
                    tokens = token_list_add(tokens, item);
                    free(token_content);
                    ls = LexerDefault;
                }
            }
            else
            {
                int t_column = column - strlen(token_content);
                Token item = create_token_from_string(token_content, TokenWord, line, t_column);
                tokens = token_list_add(tokens, item);
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
                    int t_column = column + 1 - strlen(token_content);
                    Token item = create_token_from_string(token_content, TokenNumber, line, t_column);
                    tokens = token_list_add(tokens, item);
                    free(token_content);
                    ls = LexerDefault;
                }
            }
            else
            {
                int t_column = column - strlen(token_content);
                Token item = create_token_from_string(token_content, TokenNumber, line, t_column);
                tokens = token_list_add(tokens, item);
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
        token_list_dispose(tokens);
        return NULL;
    }
    return tokens;
}