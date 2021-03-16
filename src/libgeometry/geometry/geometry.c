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

double find_polygon_area(Polygon* polygon)
{
    double sum = 0;
    for (int i = 1; i < polygon->length; i++)
    {
        sum += (polygon->points[i - 1].x * polygon->points[i].y - polygon->points[i - 1].y * polygon->points[i].x);
    }
    return fabs(sum / 2);
}

double find_polygon_perimeter(Polygon* polygon)
{
    double perimeter = 0;
    for (int i = 0; i < polygon->length; i++)
    {
        int j = (i + 1) % polygon->length;
        perimeter += sqrt(pow((polygon->points[i].x - polygon->points[j].x), 2) +
                          pow((polygon->points[i].y - polygon->points[j].y), 2));
    }
    return perimeter;
}

Figure** get_figures(char** lines, int length, int* count)
{
    return parse(lines, length, count);
}

void free_figures(Figure** figures, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (figures[i]->type == FigurePolygon)
        {
            free(((Polygon*)figures[i]->data)->points);
        }
        free(figures[i]->data);
        free(figures[i]);
    }
    free(figures);
}