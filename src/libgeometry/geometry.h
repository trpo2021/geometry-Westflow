#pragma once

typedef enum
{
    FigureCircle = 0,
    FigureTriangle = 1,
    FigureRegion = 2,
    FigureNone = -1
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
    void* data;
    FigureType type;
} Figure;

Figure** get_figures(char** lines, int length, int* count);

void free_figures(Figure** figures, int count);

double find_circle_area(Circle* circle);

double find_circle_perimeter(Circle* circle);