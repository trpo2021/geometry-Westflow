#pragma once

typedef enum
{
    FigureCircle = 0,
    FigureTriangle = 1,
    FigurePolygon = 2
} FigureType;

typedef struct
{
    double x;
    double y;
} Point;

typedef struct
{
    Point position;
    double radius;
} Circle;

typedef struct
{
    Point points[4];
} Triangle;

typedef struct
{
    int length;
    Point* points;
} Polygon;

typedef struct
{
    void* data;
    FigureType type;
} Figure;

Figure** get_figures(char** lines, int length, int* count);

void free_figures(Figure** figures, int count);

double find_circle_area(Circle* circle);

double find_circle_perimeter(Circle* circle);

double find_triangle_area(Triangle* triangle);

double find_triangle_perimeter(Triangle* triangle);

double find_polygon_area(Polygon* polygon);

double find_polygon_perimeter(Polygon* polygon);