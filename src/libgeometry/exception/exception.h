#pragma once

void print_exception(const char* data, int column);

void unexpected_token_exception(const char* token, int line, int column);

void unexpected_word_exception(const char* word);

void unknown_symbol_exception(char symbol, int line, int column);

void end_line_exception(const char* token);

void alloc_exception();