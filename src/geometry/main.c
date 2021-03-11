#include <geometry/filereader/reader.h>
#include <libgeometry/geometry.h>
#include <stdio.h>

int main(int argc, char** args)
{
    if (argc == 2)
    {
        char* data = to_lower_string(read_file(args[1]));
        int length = 0;
        char** array = split_string(&length, data, "\n\r");
        parse(array, length);
        free(array);
    }
    else
    {
        printf("Usage: ./<executable> <file path>\n");
    }
}