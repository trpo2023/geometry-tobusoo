#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 25

typedef struct point {
    double x;
    double y;
} Point;

typedef struct circle {
    Point point;
    double raduis;
    double perimeter;
    double area;
} Circle;

void to_lower_string(char* string)
{
    while (*string != '\0') {
        *string = tolower(*string);
        string++;
    }
}

double get_number(int* column)
{
    char temp[25];
    char ch;
    int point_count = 0, i = 0;

    while ((ch = getchar()) != ' ') {
        temp[i] = ch;

        if (temp[i] == '.') {
            point_count++;
            if (point_count > 1) {
                exit(EXIT_FAILURE);
            }
        }

        if (temp[i] == ')') {
            i++;
            break;
        }

        if (temp[i] == ',') {
            i++;
            break;
        }

        i++;
    }
    *column += i + 1;
    char* eptr;
    return strtod(temp, &eptr);
}

void take_info_circle(Circle* circle, int* column)
{
    circle->point.x = get_number(column);
    circle->point.y = get_number(column);
    getchar();
    circle->raduis = get_number(column);
}

void show_info_circle(Circle* circle)
{
    printf("circle(%.1f %.1f, %.1f)\n",
           circle->point.x,
           circle->point.y,
           circle->raduis);
}

int main()
{
    char geom[NAME_SIZE];
    char ch;
    int column;

    puts("Enter a geometric shape (or q for exit):");
    while ((ch = getchar()) != '\n' && ch != 'q') {
        column = 0;
        do {
            if (ch == '(') {
                if (strcmp(geom, "circle") == 0) {
                    Circle circle;
                    to_lower_string(geom);
                    take_info_circle(&circle, &column);
                    show_info_circle(&circle);
                    break;
                } else {
                    puts("Error: expected 'circle'");
                    exit(EXIT_FAILURE);
                }
            }
            geom[column++] = ch;

        } while ((ch = getchar()) != '\n');
    }

    return 0;
}