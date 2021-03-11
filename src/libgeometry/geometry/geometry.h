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