#include "stringext.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* concat_string(char* first, char* last)
{
    char* result = malloc(sizeof(char) * (strlen(first) + strlen(last) + 1));
    if (!result)
    {
        perror("malloc");
        exit(-1);
    }
    memcpy(result, first, strlen(first));
    memcpy(result + strlen(first), last, strlen(last) + 1);
    return result;
}

char* to_lower_string(char* string)
{
    for (int i = 0; string[i]; i++)
    {
        string[i] = tolower(string[i]);
    }
    return string;
}

char** split_string(int* length, char* string, char* separator)
{
    char** result = NULL;
    char* p = strtok(string, separator);
    *length = 0;
    while (p)
    {
        result = realloc(result, sizeof(char*) * ++(*length));
        if (!result)
        {
            perror("realloc");
            exit(-1);
        }
        result[(*length) - 1] = p;
        p = strtok(NULL, separator);
    }
    return result;
}

char* append_char(const char* string, char c)
{
    size_t sz = strlen(string);
    char* result = malloc(sz + 2);
    if (!result)
    {
        perror("malloc");
        exit(-1);
    }
    strcpy(result, string);
    result[sz] = c;
    result[sz + 1] = '\0';
    return result;
}