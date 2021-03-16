#include <ctype.h>
#include <errno.h>
#include <libgeometry/parser/parser.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const Token CIRCLE_TEMPLATE[] = {{.type = TokenWord},     {.type = TokenOpBracket}, {.type = TokenNumber},
                                        {.type = TokenNumber},   {.type = TokenComma},     {.type = TokenNumber},
                                        {.type = TokenClBracket}};

static const Token TRIANGLE_TEMPLATE[] = {
    {.type = TokenWord},   {.type = TokenOpBracket}, {.type = TokenOpBracket}, {.type = TokenNumber},
    {.type = TokenNumber}, {.type = TokenComma},     {.type = TokenNumber},    {.type = TokenNumber},
    {.type = TokenComma},  {.type = TokenNumber},    {.type = TokenNumber},    {.type = TokenComma},
    {.type = TokenNumber}, {.type = TokenNumber},    {.type = TokenClBracket}, {.type = TokenClBracket}};

static const Token POLYGON_TEMPLATE[] = {{.type = TokenWord},      {.type = TokenOpBracket}, {.type = TokenOpBracket},
                                         {.type = TokenNumber},    {.type = TokenNumber},    {.type = TokenComma},
                                         {.type = TokenClBracket}, {.type = TokenClBracket}};

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

static bool try_parse_template(TokenList* tokens, const char* string, FigureType type, double** result, int* length)
{
    const Token* template;
    size_t template_size;
    // min count of numbers
    unsigned int init_size;
    switch (type)
    {
    case FigureCircle:
    {
        template_size = sizeof(CIRCLE_TEMPLATE) / sizeof(CIRCLE_TEMPLATE[0]);
        template = CIRCLE_TEMPLATE;
        init_size = 3;
        *result = malloc(sizeof(double) * init_size);
    }
    break;
    case FigureTriangle:
    {
        template_size = sizeof(TRIANGLE_TEMPLATE) / sizeof(TRIANGLE_TEMPLATE[0]);
        template = TRIANGLE_TEMPLATE;
        init_size = 8;
        *result = malloc(sizeof(double) * init_size);
    }
    break;
    case FigurePolygon:
    {
        template_size = sizeof(POLYGON_TEMPLATE) / sizeof(POLYGON_TEMPLATE[0]);
        template = POLYGON_TEMPLATE;
        init_size = 8;
        *result = malloc(sizeof(double) * init_size);
    }
    break;
    }
    // back to number
    const int OFFSET_BACK = 3;
    // if time to exit
    const int OFFSET_END = 6;
    // index - index in list, index_t - in template, pos - count of numbers
    int index = 0, pos = 0;
    for (int index_t = 0; index_t < template_size; index_t++, index++)
    {
        if (index >= tokens->size)
        {
            print_exception(string, tokens->list[tokens->size - 1].column);
            end_line_exception(tokens->list[tokens->size - 1].data, template[index_t]);
            free(*result);
            return false;
        }
        if (tokens->list[index].type == template[index_t].type)
        {
            if (type == FigurePolygon && tokens->list[index].type == TokenComma)
            {
                index_t -= OFFSET_BACK;
            }
            if (tokens->list[index].type == TokenNumber)
            {
                if (init_size <= pos)
                {
                    *result = realloc(*result, sizeof(double) * (init_size *= 2));
                }
                if (!try_parse_number(tokens->list[index].data, &((*result)[pos++])))
                {
                    print_exception(string, tokens->list[index].column);
                    unexpected_token_exception(tokens->list[index], "<double>");
                    free(*result);
                    return false;
                }
            }
        }
        else if (type == FigurePolygon && tokens->list[index].type == TokenClBracket &&
                 tokens->list[index - 1].type == TokenNumber && tokens->list[index - 2].type == TokenNumber)
        {
            index_t = OFFSET_END;
            continue;
        }
        else
        {
            print_exception(string, tokens->list[index].column);
            unexpected_token_exception(tokens->list[index], NULL);
            free(*result);
            return false;
        }
    }
    if (index != tokens->size)
    {
        print_exception(string, tokens->list[index].column);
        unexpected_token_exception(tokens->list[index], NULL);
        free(*result);
        return false;
    }
    if (length)
    {
        *length = pos;
    }
    return true;
}

static Circle* create_circle(double* numbers)
{
    Circle* circle = NULL;
    circle = malloc(sizeof(Circle));
    if (!circle)
    {
        alloc_exception();
        exit(-1);
    }
    circle->position.x = numbers[0];
    circle->position.y = numbers[1];
    circle->radius = numbers[2];
    if (circle->radius < 0)
    {
        printf("Wrong circle. Radius must be unsigned double.\n");
        free(circle);
        return NULL;
    }
    return circle;
}

static Triangle* create_triangle(double* numbers)
{
    Triangle* triangle = malloc(sizeof(Triangle));
    if (!triangle)
    {
        alloc_exception();
        exit(-1);
    }
    for (int i = 0, n = 0; i < sizeof(triangle->points) / sizeof(triangle->points[0]); i++, n += 2)
    {
        triangle->points[i].x = numbers[n];
        triangle->points[i].y = numbers[n + 1];
    }
    if (triangle->points[0].x != triangle->points[3].x || triangle->points[0].y != triangle->points[3].y)
    {
        printf("Wrong triangle. Last point must be equal to the first point.\n");
        free(triangle);
        return NULL;
    }
    return triangle;
}

static Polygon* create_polygon(double* numbers, int length)
{
    Polygon* polygon = malloc(sizeof(Polygon));
    if (!polygon)
    {
        alloc_exception();
        exit(-1);
    }
    polygon->length = length / 2;
    if (polygon->length < 4)
    {
        printf("Wrong polygon. Must be at least 4 points.\n");
        free(polygon);
        return NULL;
    }
    polygon->points = malloc(sizeof(Point) * polygon->length);
    if (!polygon->points)
    {
        alloc_exception();
        exit(-1);
    }
    for (int i = 0, n = 0; i < polygon->length; i++, n += 2)
    {
        polygon->points[i].x = numbers[n];
        polygon->points[i].y = numbers[n + 1];
    }
    if (polygon->points[0].x != polygon->points[polygon->length - 1].x ||
        polygon->points[0].y != polygon->points[polygon->length - 1].y)
    {
        printf("Wrong polygon. Last point must be equal to the first point.\n");
        free(polygon->points);
        free(polygon);
        return NULL;
    }
    return polygon;
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
            double* numbers = NULL;
            if (try_parse_template(tokens, string, FigureCircle, &numbers, NULL))
            {
                Circle* circle = create_circle(numbers);
                free(numbers);
                if (circle)
                {
                    return create_figure(FigureCircle, circle);
                }
            }
            return NULL;
        }
        else if (strcmp(tokens->list[0].data, "triangle") == 0)
        {
            double* numbers = NULL;
            if (try_parse_template(tokens, string, FigureTriangle, &numbers, NULL))
            {
                Triangle* triangle = create_triangle(numbers);
                free(numbers);
                if (triangle)
                {
                    return create_figure(FigureTriangle, triangle);
                }
            }
            return NULL;
        }
        else if (strcmp(tokens->list[0].data, "polygon") == 0)
        {
            double* numbers = NULL;
            int lentgh = 0;
            if (try_parse_template(tokens, string, FigurePolygon, &numbers, &lentgh))
            {
                Polygon* polygon = create_polygon(numbers, lentgh);
                free(numbers);
                if (polygon)
                {
                    return create_figure(FigurePolygon, polygon);
                }
            }
            return NULL;
        }
    }
    print_exception(string, tokens->list[0].column);
    unexpected_token_exception(tokens->list[0], "'circle', 'triangle' or 'polygon'");
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