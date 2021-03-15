#include <libgeometry/exception/exception.h>
#include <stdio.h>

void print_exception(const char* data, int column)
{
    printf("%s\n", data);
    for (int i = 0; i < column; i++)
    {
        printf(" ");
    }
    printf("^\n");
}

void unexpected_token_exception(const char* token, int line, int column)
{
    printf("Unexpected token <%s> at line %d and column %d\n", token, line, column);
}

void unexpected_word_exception(const char* word)
{
    printf("Unexpected word '%s'.\n", word);
}

void unknown_symbol_exception(char symbol, int line, int column)
{
    printf("Unexpected symbol %c at line %d and column %d.\n", symbol, line, column);
}

void end_line_exception(const char* token)
{
    printf("Unexpected end of line on token <%s>\n", token);
}

void alloc_exception()
{
    printf("Unable to allocate memory. Exit.\n");
}