#include <geometry/filereader/reader.h>
#include <libgeometry/geometry.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** args)
{
    if (argc == 2)
    {
        char* data = to_lower_string(read_file(args[1]));
        int length = 0;
        char** array = split_string(&length, data, "\n\r");
        int count = 0;
        Figure** figures = get_figures(array, length, &count);
        free(array);
        for (int i = 0; i < count; i++)
        {
            switch (figures[i]->type)
            {
            case FigureCircle:
            {
                Circle* circle = (Circle*)figures[i]->data;
                printf("FIGURE: Circle.\n");
                printf("Point: %lf, %lf.\nRadius: %lf.\n", circle->position.x, circle->position.y, circle->radius);
                printf("Area: %lf\n", find_circle_area(circle));
                printf("Perimeter: %lf\n", find_circle_perimeter(circle));
            }
            break;
            case FigureTriangle:
            {
                Triangle* triangle = (Triangle*)figures[i]->data;
                printf("FIGURE: Triangle.\n");
                for (int i = 0; i < sizeof(triangle->points) / sizeof(triangle->points[0]); i++)
                {
                    printf("Point: %lf, %lf.\n", triangle->points[i].x, triangle->points[i].y);
                }
                printf("Area: %lf\n", find_triangle_area(triangle));
                printf("Perimeter: %lf\n", find_triangle_perimeter(triangle));
            }
            break;
            case FigureRegion:
            {
            }
            break;
            case FigureNone:
            {
            }
            break;
            }
            printf("\n");
        }
        free_figures(figures, count);
    }
    else
    {
        printf("Usage: ./<executable> <file path>\n");
    }
}