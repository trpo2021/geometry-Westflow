#pragma once

#include <libgeometry/geometry.h>
#include <libgeometry/lexer/lexer.h>

Figure** parse(char** lines, int length, int* fcount);