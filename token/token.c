#include "token.h"
#include <stdio.h>

Token create_token_from_char(char c, int line, int column)
{
    char* data = "";
    data = append_char(data, c);
    Token token = {
        .data = data, .type = (TokenType)c, .line = line, .column = column};
    return token;
}

Token create_token_from_string(char* string, TokenType type, int line,
                               int column)
{
    Token token = {.data = clone_string(string),
                   .type = type,
                   .line = line,
                   .column = column};
    return token;
}

TokenList* token_list_init(TokenList* this)
{
    this = malloc(sizeof(TokenList));
    Token* list;
    list = malloc(sizeof(Token));
    if (!list)
    {
        perror("malloc");
        free(list);
        exit(-1);
    }
    else
    {
        this->list = list;
        this->size = 0;
    }
    return this;
}

TokenList* token_list_add(TokenList* this, Token item)
{
    Token* list;
    this->size += 1;
    list = realloc(this->list, this->size * sizeof(Token));
    if (!list)
    {
        perror("realloc");
        free(list);
        exit(-1);
    }
    else
    {
        this->list = list;
        this->list[this->size - 1] = item;
    }
    return this;
}

void token_list_remove(TokenList* this, int index)
{
    TokenList temp;
    Token* pointer;
    token_list_init(&temp);
    for (int i = index; i < this->size; i++)
    {
        this->list[i] = this->list[i + 1];
    }
    this->size -= 1;
    for (int i = 0; i < this->size; i++)
    {
        token_list_add(&temp, this->list[i]);
    }
    pointer = realloc(temp.list, temp.size * sizeof(Token));
    if (!pointer)
    {
        perror("realloc");
        free(pointer);
        exit(-1);
    }
    this->list = pointer;
}

void token_list_dispose(TokenList* this)
{
    for (int i = 0; i < this->size; i++)
    {
        free(this->list[i].data);
        // free(this->list[i]);
    }
    free(this->list);
    free(this);
}