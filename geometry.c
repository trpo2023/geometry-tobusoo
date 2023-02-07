#include <ctype.h>
#include <stdbool.h>
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

enum Errors {
    ER_NAME,
    ER_NOT_DOUBLE,
    ER_BACK_BRACE,
    ER_UNEXPECT_TOKEN,
    ER_EXPECT_COMMA,
    ER_UNEXPECT_COMMA
};

void print_error(int column, int status)
{
    for (int i = 0; i < column; i++) {
        putchar(' ');
    }
    printf("^\n");
    switch (status) {
    case ER_NAME:
        printf("Error at column %d: expected 'circle'\n", column);
        break;
    case ER_NOT_DOUBLE:
        printf("Error at column %d: expected '<double>'\n", column);
        break;
    case ER_BACK_BRACE:
        printf("Error at column %d: expected ')'\n", column);
        break;
    case ER_UNEXPECT_TOKEN:
        printf("Error at column %d: unexpected token\n", column);
        break;
    case ER_EXPECT_COMMA:
        printf("Error at column %d: expected ','\n", column);
        break;
    case ER_UNEXPECT_COMMA:
        printf("Error at column %d: unexpected ','\n", column);
    }
}

void to_lower_string(char* string)
{
    while (*string != '\0') {
        *string = tolower(*string);
        string++;
    }
}

void del_space(int* column)
{
    char ch;
    while ((ch = getchar()) == ' ') {
        *column += 1;
        continue;
    }
    if (ch != ' ')
        ungetc(ch, stdin);
}

double get_number(int* column)
{
    char temp[25];
    char ch;
    int point_count = 0, i = 0;

    del_space(column);

    while ((ch = getchar()) != ' ') {
        temp[i] = ch;

        if (temp[i] == '.') {
            point_count++;
            if (point_count > 1) {
                print_error(*column + i + 1, ER_NOT_DOUBLE);
                exit(EXIT_FAILURE);
            }
        }

        if (temp[i] == ')') {
            ungetc(temp[i], stdin);
            i++;
            break;
        }

        if (temp[i] == ',') {
            ungetc(temp[i], stdin);
            break;
        }

        if (temp[i] == '(') {
            i++;
            print_error(*column + i, ER_BACK_BRACE);
            exit(EXIT_FAILURE);
        }

        if (!isdigit(temp[i]) && temp[i] != '.') {
            i++;
            print_error(*column + i, ER_NOT_DOUBLE);
            exit(EXIT_FAILURE);
        }

        i++;
    }
    del_space(column);
    *column += i + 1;
    char* eptr;
    return strtod(temp, &eptr);
}

bool expect(char expect, int* column, int status)
{
    char ch;
    if ((ch = getchar()) == expect) {
        return true;
    } else {
        print_error(*column, status);
        exit(EXIT_FAILURE);
    }
}

bool unexpect(char unexpect, int* column, int status)
{
    char ch;
    if ((ch = getchar()) == unexpect) {
        print_error(*column, status);
        exit(EXIT_FAILURE);
    }
    ungetc(ch, stdin);
    return true;
}

void get_point(Point* point, int* column)
{
    point->x = get_number(column);
    unexpect(',', column, ER_UNEXPECT_COMMA);

    point->y = get_number(column);
    expect(',', column, ER_EXPECT_COMMA);
}

void end_of_line(int* column)
{
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (ch != ' ') {
            print_error(*column, ER_UNEXPECT_TOKEN);
            exit(EXIT_FAILURE);
        }
        *column += 1;
    }
}

void take_info_circle(Circle* circle, int* column)
{
    get_point(&circle->point, column);

    circle->raduis = get_number(column);

    expect(')', column, ER_BACK_BRACE);

    end_of_line(column);
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
    while ((ch = getchar()) != EOF && ch != 'q') {
        column = 0;
        do {
            if (ch == '(' || ch == ' ') {
                to_lower_string(geom);
                if (strcmp(geom, "circle") == 0) {
                    Circle circle;
                    take_info_circle(&circle, &column);
                    show_info_circle(&circle);
                    break;
                } else {
                    print_error(0, ER_NAME);
                    exit(EXIT_FAILURE);
                }
            }

            if (ch == ')') {
                print_error(column, ER_BACK_BRACE);
                exit(EXIT_FAILURE);
            }

            geom[column++] = ch;

        } while ((ch = getchar()) != '\n');
        puts("Enter a new geometric shape (or q for exit):");
    }

    return 0;
}