#include <libgeometry/geometry.h>
#include <libgeometry/parser/parser.h>
#define _USE_MATH_DEFINES
#include <math.h>

double find_circle_area(Circle* circle)
{
    return pow(circle->radius, 2) * M_PI;
}

double find_circle_perimeter(Circle* circle)
{
    return circle->radius * 2 * M_PI;
}

Figure** get_figures(char** lines, int length, int* count)
{
    return parse(lines, length, count);
}

void free_figures(Figure** figures, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(figures[i]->data);
        free(figures[i]);
    }
    free(figures);
}