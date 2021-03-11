#pragma once

#include <libgeometry/exception/exception.h>
#include <libgeometry/token/token.h>
#include <stdlib.h>

typedef enum
{
    LexerDefault,
    LexerWord,
    LexerNumber
} LexerState;

TokenList* lex(const char* data, size_t length, int line);