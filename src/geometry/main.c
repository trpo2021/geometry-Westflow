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
                printf("FIGURE: Circle. Point: %lf, %lf. Radius: %lf\n",
                       ((Circle*)figures[i]->data)->position.x,
                       ((Circle*)figures[i]->data)->position.y,
                       ((Circle*)figures[i]->data)->radius);
                printf("Area: %lf\n",
                       find_circle_area(((Circle*)figures[i]->data)));
                printf("Perimeter: %lf\n",
                       find_circle_perimeter(((Circle*)figures[i]->data)));
                break;
            case FigureTriangle:
                break;
            case FigureRegion:
                break;
            case FigureNone:
                break;
            }
        }
        free_figures(figures, count);
    }
    else
    {
        printf("Usage: ./<executable> <file path>\n");
    }
}