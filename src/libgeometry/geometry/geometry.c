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

double find_triangle_area(Triangle* triangle)
{
    double determinant =
        ((triangle->points[1].x - triangle->points[0].x) * (triangle->points[2].y - triangle->points[0].y)) -
        ((triangle->points[2].x - triangle->points[0].x) * (triangle->points[1].y - triangle->points[0].y));
    return fabs(determinant) / 2;
}

double find_triangle_perimeter(Triangle* triangle)
{
    double ab = sqrt(pow(triangle->points[1].x - triangle->points[0].x, 2) +
                     pow(triangle->points[1].y - triangle->points[0].y, 2));
    double bc = sqrt(pow(triangle->points[2].x - triangle->points[1].x, 2) +
                     pow(triangle->points[2].y - triangle->points[1].y, 2));
    double ac = sqrt(pow(triangle->points[2].x - triangle->points[0].x, 2) +
                     pow(triangle->points[2].y - triangle->points[0].y, 2));
    return ab + bc + ac;
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