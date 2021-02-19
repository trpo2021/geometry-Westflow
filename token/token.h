#pragma once

#include "../Strings/stringext.h"
#include <stdlib.h>

typedef enum
{
    TokenWord = 0,
    TokenNumber = 1,
    TokenOpBracket = '(',
    TokenComma = ',',
    TokenClBracket = ')'
} TokenType;

typedef struct
{
    char* data;
    TokenType type;
    unsigned int column;
    unsigned int line;
} Token;

typedef struct
{
    Token* list;
    size_t size;
} TokenList;

Token create_token_from_char(char c, int line, int column);

Token create_token_from_string(char* string, TokenType type, int line,
                               int column);

TokenList* token_list_init(TokenList* this);

TokenList* token_list_add(TokenList* this, Token item);

void token_list_remove(TokenList* this, int index);

void token_list_dispose(TokenList* this);
