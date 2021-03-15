#include <ctype.h>
#include <libgeometry/exception/exception.h>
#include <libgeometry/strings/stringext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* clone_string(const char* string)
{
    char* clone = malloc(strlen(string) + 1);
    memcpy(clone, string, strlen(string) + 1);
    return clone;
}

char* concat_string(char* first, char* last)
{
    char* result = malloc(sizeof(char) * (strlen(first) + strlen(last) + 1));
    if (!result)
    {
        alloc_exception();
        exit(-1);
    }
    memcpy(result, first, strlen(first));
    memcpy(result + strlen(first), last, strlen(last) + 1);
    if (strlen(first) > 0)
    {
        free(first);
    }
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
            alloc_exception();
            exit(-1);
        }
        result[(*length) - 1] = p;
        p = strtok(NULL, separator);
    }
    return result;
}

char* append_char(char* string, char c)
{
    size_t sz = strlen(string);
    char* result = NULL;
    if (sz == 0)
    {
        result = malloc(2);
    }
    else
    {
        result = realloc(string, sz + 2);
    }
    if (!result)
    {
        alloc_exception();
        exit(-1);
    }
    result[sz] = c;
    result[sz + 1] = '\0';
    return result;
}