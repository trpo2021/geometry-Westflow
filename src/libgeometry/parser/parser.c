#include <ctype.h>
#include <errno.h>
#include <libgeometry/parser/parser.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const Token CIRCLE_TEMPLATE[] = {{.type = TokenWord},     {.type = TokenOpBracket}, {.type = TokenNumber},
                                 {.type = TokenNumber},   {.type = TokenComma},     {.type = TokenNumber},
                                 {.type = TokenClBracket}};

static bool try_parse_number(const char* string, double* result)
{
    char* error = NULL;
    errno = 0;
    *result = strtod(string, &error);
    if (errno != 0 || *error != '\0')
    {
        return false;
    }
    return true;
}

static Circle* create_circle(TokenList* tokens, const char* string)
{
    Circle* circle = NULL;
    double numbers[3];
    size_t template_size = sizeof(CIRCLE_TEMPLATE) / sizeof(CIRCLE_TEMPLATE[0]);
    for (int i = 0, pos = 0; i < template_size; i++)
    {
        if (i >= tokens->size)
        {
            print_exception(string, tokens->list[tokens->size - 1].column);
            end_line_exception(tokens->list[tokens->size - 1].data);
            return NULL;
        }
        if (tokens->list[i].type == CIRCLE_TEMPLATE[i].type)
        {
            if (tokens->list[i].type == TokenNumber)
            {
                if (!try_parse_number(tokens->list[i].data, &numbers[pos++]))
                {
                    print_exception(string, tokens->list[i].column);
                    unexpected_token_exception(tokens->list[i].data, tokens->list[i].line, tokens->list[i].column);
                    return NULL;
                }
            }
        }
        else
        {
            print_exception(string, tokens->list[i].column);
            unexpected_token_exception(tokens->list[i].data, tokens->list[i].line, tokens->list[i].column);
            return NULL;
        }
    }
    if (tokens->size > template_size)
    {
        print_exception(string, tokens->list[template_size].column);
        unexpected_token_exception(tokens->list[template_size].data, tokens->list[template_size].line,
                                   tokens->list[template_size].column);
        return NULL;
    }
    circle = malloc(sizeof(Circle));
    if (!circle)
    {
        alloc_exception();
        exit(-1);
    }
    circle->position.x = numbers[0];
    circle->position.y = numbers[1];
    circle->radius = numbers[2];
    return circle;
}

static Figure* create_figure(FigureType type, void* data)
{
    Figure* figure = NULL;
    figure = malloc(sizeof(Figure));
    if (!figure)
    {
        alloc_exception();
        exit(-1);
    }
    figure->type = type;
    figure->data = data;
    return figure;
}

static Figure* parse_line(TokenList* tokens, const char* string)
{
    if (tokens == NULL)
    {
        return NULL;
    }
    if (tokens->list[0].type == TokenWord)
    {
        if (strcmp(tokens->list[0].data, "circle") == 0)
        {
            Circle* circle = create_circle(tokens, string);
            if (circle)
            {
                return create_figure(FigureCircle, circle);
            }
            return NULL;
        }
        print_exception(string, tokens->list[0].column);
        unexpected_word_exception(tokens->list[0].data);
        return NULL;
    }
    print_exception(string, tokens->list[0].column);
    unexpected_token_exception(tokens->list[0].data, tokens->list[0].line, tokens->list[0].column);
    return NULL;
}

Figure** parse(char** lines, int length, int* fcount)
{
    Figure** figures = malloc(sizeof(Figure));
    if (!figures)
    {
        exit(-1);
    }
    int count = 1;
    for (int line = 0; line < length; line++)
    {
        TokenList* tokens = lex(lines[line], strlen(lines[line]), line + 1);
        Figure* figure = parse_line(tokens, lines[line]);
        if (tokens)
        {
            token_list_dispose(tokens);
        }
        if (figure)
        {
            figures[count - 1] = figure;
            Figure** temp = realloc(figures, sizeof(figures) * (++count));
            if (!temp)
            {
                exit(-1);
            }
            figures = temp;
        }
    }
    *(fcount) = count - 1;
    return figures;
}