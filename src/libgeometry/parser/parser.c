#include <ctype.h>
#include <errno.h>
#include <libgeometry/parser/parser.h>
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
    char* error = NULL;
    errno = 0;
    *result = strtod(string, &error);
    if (errno != 0 || *error != '\0')
        return false;
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