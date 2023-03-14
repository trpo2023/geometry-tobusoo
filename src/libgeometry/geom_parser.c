#include <ctype.h>
#include <libgeometry/geom_parser.h>
#include <libgeometry/lexer.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 25

void take_info_circle(Circle* circle, int* column, int is_file, FILE* file)
{
    get_point(&circle->point, column, is_file, file);
    expect(',', column, ER_EXPECT_COMMA, is_file, file);

    circle->radius = get_number(column, is_file, file);

    if (circle->radius < 0) {
        print_error(*column - 2, ER_NEGATIVE_R, is_file, file);
        exit(EXIT_FAILURE);
    }

    expect(')', column, ER_BACKSLASH, is_file, file);

    end_of_line(column, is_file, file);

    circle->perimeter = 2 * M_PI * circle->radius;
    circle->area = M_PI * circle->radius * circle->radius;
}

void show_info_circle(Circle* circle)
{
    printf("circle(%.2f %.2f, %.2f)\n",
           circle->point.x,
           circle->point.y,
           circle->radius);
    printf("\tarea = %.4f\n", circle->area);
    printf("\tperimeter = %.4f\n", circle->perimeter);
}

void take_info_triangle(Triangle* tr, int* column, int is_file, FILE* file)
{
    expect('(', column, ER_BACKSLASH, is_file, file);
    get_point(&tr->p1, column, is_file, file);
    expect(',', column, ER_EXPECT_COMMA, is_file, file);
    get_point(&tr->p2, column, is_file, file);
    expect(',', column, ER_EXPECT_COMMA, is_file, file);
    get_point(&tr->p3, column, is_file, file);
    expect(',', column, ER_EXPECT_COMMA, is_file, file);
    get_point(&tr->p4, column, is_file, file);
    expect(')', column, ER_BACKSLASH, is_file, file);
    expect(')', column, ER_BACKSLASH, is_file, file);

    end_of_line(column, is_file, file);

    double area_l, area_r;
    area_l = (tr->p2.x - tr->p1.x) * (tr->p3.y - tr->p1.y);
    area_r = (tr->p3.x - tr->p1.x) * (tr->p2.y - tr->p1.y);
    tr->area = 0.5 * abs(area_l - area_r);

    double len_1, len_2, len_3;
    len_1 = sqrt(pow(tr->p1.x - tr->p2.x, 2) + pow(tr->p1.y - tr->p2.y, 2));
    len_2 = sqrt(pow(tr->p1.x - tr->p3.x, 2) + pow(tr->p1.y - tr->p3.y, 2));
    len_3 = sqrt(pow(tr->p2.x - tr->p3.x, 2) + pow(tr->p2.y - tr->p3.y, 2));
    tr->perimeter = len_1 + len_2 + len_3;
}

void show_info_triangle(Triangle* tr)
{
    printf("triangle((%.1f %.1f, %.1f %.1f, %.1f %.1f, %.1f %.1f))\n",
           tr->p1.x,
           tr->p1.y,
           tr->p2.x,
           tr->p2.y,
           tr->p3.x,
           tr->p3.y,
           tr->p4.x,
           tr->p4.y);
    printf("\tarea = %.3f\n", tr->area);
    printf("\tperimeter = %.3f\n", tr->perimeter);
}

void parser(FILE* file, int is_file)
{
    char geom[NAME_SIZE] = {0};
    char ch;
    int column;

    if (is_file == NOT_FILE)
        puts("Enter a geometric shape (or q for exit):");
    while ((ch = getc(file)) != EOF && ch != 'q') {
        column = 0;
        do {
            if (ch == '(' || ch == ' ') {
                to_lower_string(geom);
                column++;
                if (strcmp(geom, "circle") == 0) {
                    Circle circle;
                    take_info_circle(&circle, &column, is_file, file);
                    printf("\nYou have entered: \n");
                    show_info_circle(&circle);
                    break;
                } else if (strcmp(geom, "triangle") == 0) {
                    Triangle triangle;
                    take_info_triangle(&triangle, &column, is_file, file);
                    printf("\nYou have entered: \n");
                    show_info_triangle(&triangle);
                    break;
                } else {
                    print_error(0, ER_NAME, is_file, file);
                    exit(EXIT_FAILURE);
                }
            }

            if (ch == ')') {
                print_error(column, ER_BACKSLASH, is_file, file);
                exit(EXIT_FAILURE);
            }

            geom[column++] = ch;

        } while ((ch = getc(file)) != '\n');

        for (int i = 0; i < NAME_SIZE; i++) {
            geom[i] = '\0';
        }

        if (is_file == NOT_FILE)
            puts("Enter a new geometric shape (or q for exit):");
    }
}
