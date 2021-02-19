#include "reader.h"
#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* filename)
{
    FILE* stream;
    char* outp = "";
    char* line = NULL;
    size_t len = 0;
    size_t nread;
    stream = fopen(filename, "r");
    if (!stream)
    {
        perror("fopen");
        exit(-2);
    }
    while ((nread = getline(&line, &len, stream)) != -1)
    {
        outp = concat_string(outp, line);
    }
    free(line);
    fclose(stream);
    return outp;
}