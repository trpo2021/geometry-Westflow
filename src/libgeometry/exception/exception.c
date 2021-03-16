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

void unexpected_token_exception(Token unexpected, char* expected)
{
    printf("Unexpected token <%s> at line %d and column %d\n", unexpected.data, unexpected.line, unexpected.column);
    if (expected)
    {
        printf("Expected: %s.\n", expected);
    }
}

void unknown_symbol_exception(char symbol, int line, int column)
{
    printf("Unexpected symbol %c at line %d and column %d.\n", symbol, line, column);
}

void end_line_exception(const char* token, Token expected)
{
    printf("Unexpected end of line on token <%s>\n", token);
    if (expected.type == TokenNumber)
    {
        printf("Expected next: <double>.\n");
    }
    else
    {
        printf("Expected next: <%c>.\n", expected.type);
    }
}

void alloc_exception()
{
    printf("Unable to allocate memory. Exit.\n");
}