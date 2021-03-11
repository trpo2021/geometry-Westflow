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