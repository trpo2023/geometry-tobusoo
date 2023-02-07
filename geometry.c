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

int main()
{
    char geom[NAME_SIZE];
    char ch;
    int column = 0;

    puts("Enter a geometric shape (or q for exit):");
    while ((ch = getchar()) != '\n' && ch != 'q') {
        do {
            if (ch == '(' || ch == ' ') {
                if (strcmp(geom, "circle") == 0) {
                    to_lower_string(geom);
                    puts(geom);
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