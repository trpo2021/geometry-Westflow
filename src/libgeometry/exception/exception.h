#pragma once

#include <libgeometry/token/token.h>

void print_exception(const char* data, int column);

void unexpected_token_exception(Token unexpected, char* expected);

void unknown_symbol_exception(char symbol, int line, int column);

void end_line_exception(const char* token, Token expected);

void alloc_exception();