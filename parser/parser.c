#include "parser.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const Token CIRCLE_TEMPLATE[] = {{.data = "circle", .type = TokenWord},
                                 {.data = "(", .type = TokenOpBracket},
                                 {.data = "double", .type = TokenNumber},
                                 {.data = "double", .type = TokenNumber},
                                 {.data = ",", .type = TokenComma},
                                 {.data = "double", .type = TokenNumber},
                                 {.data = ")", .type = TokenClBracket}};

static bool try_parse_number(const char* string, double* result)
{
    bool has_dot = false;
    if (isdigit(string[0]) || (string[0] == '-' && isdigit(string[1])))
    {
        for (int i = 1; i < strlen(string); i++)
        {
            if (string[i] == '.' && i + 1 < strlen(string))
            {
                if (has_dot)
                {
                    return false;
                }
                has_dot = true;
            }
            else if (!isdigit(string[i]))
            {
                return false;
            }
        }
        *result = strtod(string, NULL);
        return true;
    }
    return false;
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
            printf("Unexpected end of line. Expected <%s>\n",
                   CIRCLE_TEMPLATE[i].data);
            printf("Use the following signature: <circle><(>[ ]<double>[ "
                   "]<double>[ ]<,>[ ]<double>[ ]<)>\n");
            return NULL;
        }
        if (tokens->list[i].type == CIRCLE_TEMPLATE[i].type)
        {
            if (tokens->list[i].type == TokenNumber)
            {
                if (!try_parse_number(tokens->list[i].data, &numbers[pos++]))
                {
                    print_exception(string, tokens->list[i].column);
                    printf("Unexpected token <%s> at line %d and column %d, "
                           "expected <double>\n",
                           tokens->list[i].data, tokens->list[i].line,
                           tokens->list[i].column);
                    return NULL;
                }
            }
        }
        else
        {
            print_exception(string, tokens->list[i].column);
            printf("Unexpected token <%s> at line %d and column %d, expected "
                   "<%s>\n",
                   tokens->list[i].data, tokens->list[i].line,
                   tokens->list[i].column, CIRCLE_TEMPLATE[i].data);
            return NULL;
        }
    }
    if (tokens->size > template_size)
    {
        print_exception(string, tokens->list[template_size].column);
        printf("Unexpected token <%s> at line %d and column %d\n",
               tokens->list[template_size].data,
               tokens->list[template_size].line,
               tokens->list[template_size].column);
        return NULL;
    }
    circle = malloc(sizeof(Circle));
    if (!circle)
    {
        perror("malloc");
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
        perror("malloc");
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
        if (strcmp(tokens->list[0].data, CIRCLE_TEMPLATE[0].data) == 0)
        {
            Circle* circle = create_circle(tokens, string);
            if (circle)
            {
                return create_figure(FigureCircle, circle);
            }
            return NULL;
        }
        print_exception(string, tokens->list[0].column);
        printf("Unexpected word '%s'. Expected 'circle'\n",
               tokens->list[0].data);
        return NULL;
    }
    print_exception(string, tokens->list[0].column);
    printf("Unexpected token <%s>. Expected word.\n", tokens->list[0].data);
    return NULL;
}

void parse(char** lines, int length)
{
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
            Circle* cir = (Circle*)figure->data;
            printf("\n%d)\tCircle. Point: [%lf, %lf], Radius: %lf\n\n",
                   line + 1, cir->position.x, cir->position.y, cir->radius);
            free(figure);
        }
    }
}