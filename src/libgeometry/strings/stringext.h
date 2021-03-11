#pragma once

char* create_string(unsigned int size);

char* concat_string(char* first, char* last);

char* to_lower_string(char* string);

char** split_string(int* length, char* string, char* separator);

char* append_char(char* string, char c);

char* clone_string(const char* string);