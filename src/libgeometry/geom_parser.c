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
    if (is_file == _FILE) {
        get_point(&circle->point, column, _FILE, file);
        expect(',', column, ER_EXPECT_COMMA, _FILE, file);

        circle->radius = get_number(column, _FILE, file);

        if (circle->radius < 0) {
            print_error(*column - 2, ER_NEGATIVE_R, _FILE, file);
            exit(EXIT_FAILURE);
        }

        expect(')', column, ER_BACKSLASH, _FILE, file);

        end_of_line(column, _FILE, file);
    } else {
        get_point(&circle->point, column, NOT_FILE, file);
        expect(',', column, ER_EXPECT_COMMA, NOT_FILE, file);

        circle->radius = get_number(column, NOT_FILE, file);

        if (circle->radius < 0) {
            print_error(*column - 2, ER_NEGATIVE_R, NOT_FILE, file);
            exit(EXIT_FAILURE);
        }

        expect(')', column, ER_BACKSLASH, NOT_FILE, file);

        end_of_line(column, NOT_FILE, file);
    }
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
    if (is_file == NOT_FILE) {
        expect('(', column, ER_BACKSLASH, NOT_FILE, file);
        get_point(&tr->p1, column, NOT_FILE, file);
        expect(',', column, ER_EXPECT_COMMA, NOT_FILE, file);
        get_point(&tr->p2, column, NOT_FILE, file);
        expect(',', column, ER_EXPECT_COMMA, NOT_FILE, file);
        get_point(&tr->p3, column, NOT_FILE, file);
        expect(',', column, ER_EXPECT_COMMA, NOT_FILE, file);
        get_point(&tr->p4, column, NOT_FILE, file);
        expect(')', column, ER_BACKSLASH, NOT_FILE, file);
        expect(')', column, ER_BACKSLASH, NOT_FILE, file);

        end_of_line(column, NOT_FILE, file);
    } else {
        expect('(', column, ER_BACKSLASH, _FILE, file);
        get_point(&tr->p1, column, _FILE, file);
        expect(',', column, ER_EXPECT_COMMA, _FILE, file);
        get_point(&tr->p2, column, _FILE, file);
        expect(',', column, ER_EXPECT_COMMA, _FILE, file);
        get_point(&tr->p3, column, _FILE, file);
        expect(',', column, ER_EXPECT_COMMA, _FILE, file);
        get_point(&tr->p4, column, _FILE, file);
        expect(')', column, ER_BACKSLASH, _FILE, file);
        expect(')', column, ER_BACKSLASH, _FILE, file);

        end_of_line(column, _FILE, file);
    }

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

void parser_stdin(FILE* stdin)
{
    char geom[NAME_SIZE] = {0};
    char ch;
    int column;

    puts("Enter a geometric shape (or q for exit):");
    while ((ch = getc(stdin)) != EOF && ch != 'q') {
        column = 0;
        do {
            if (ch == '(' || ch == ' ') {
                to_lower_string(geom);
                column++;
                if (strcmp(geom, "circle") == 0) {
                    Circle circle;
                    take_info_circle(&circle, &column, NOT_FILE, stdin);
                    printf("\nYou have entered: \n");
                    show_info_circle(&circle);
                    break;
                } else if (strcmp(geom, "triangle") == 0) {
                    Triangle triangle;
                    take_info_triangle(&triangle, &column, NOT_FILE, stdin);
                    printf("\nYou have entered: \n");
                    show_info_triangle(&triangle);
                    break;
                } else {
                    print_error(0, ER_NAME, NOT_FILE, stdin);
                    exit(EXIT_FAILURE);
                }
            }

            if (ch == ')') {
                print_error(column, ER_BACKSLASH, NOT_FILE, stdin);
                exit(EXIT_FAILURE);
            }

            geom[column++] = ch;

        } while ((ch = getc(stdin)) != '\n');

        for (int i = 0; i < NAME_SIZE; i++) {
            geom[i] = '\0';
        }

        puts("Enter a new geometric shape (or q for exit):");
    }
}

void parser_file(FILE* file)
{
    char geom[NAME_SIZE] = {0};
    char ch;
    int column;

    while ((ch = getc(file)) != EOF && ch != 'q') {
        column = 0;
        do {
            if (ch == '(' || ch == ' ') {
                to_lower_string(geom);
                column++;
                if (strcmp(geom, "circle") == 0) {
                    Circle circle;
                    take_info_circle(&circle, &column, _FILE, file);
                    printf("\nYou have entered: \n");
                    show_info_circle(&circle);
                    break;
                } else if (strcmp(geom, "triangle") == 0) {
                    Triangle triangle;
                    take_info_triangle(&triangle, &column, _FILE, file);
                    printf("\nYou have entered: \n");
                    show_info_triangle(&triangle);
                    break;
                } else {
                    print_error(0, ER_NAME, _FILE, file);
                    exit(EXIT_FAILURE);
                }
            }

            if (ch == ')') {
                print_error(column, ER_BACKSLASH, _FILE, file);
                exit(EXIT_FAILURE);
            }

            geom[column++] = ch;

        } while ((ch = getc(file)) != '\n');

        for (int i = 0; i < NAME_SIZE; i++) {
            geom[i] = '\0';
        }
    }
}