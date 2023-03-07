#pragma once
#include <stdio.h>
#include <stdlib.h>

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

typedef struct triangle {
    Point p1;
    Point p2;
    Point p3;
    Point p4;
    double perimeter;
    double area;
} Triangle;

enum Errors {
    _FILE,
    NOT_FILE,
    ER_NAME,
    ER_NOT_DOUBLE,
    ER_BACKSLASH,
    ER_UNEXPECT_TOKEN,
    ER_EXPECT_COMMA,
    ER_UNEXPECT_COMMA,
};

void show_info_triangle(Triangle* tr);
void take_info_triangle(Triangle* tr, int* column, int is_file, FILE* file);
void parser_file(FILE* file);
void parser_stdin(FILE* stdin);
void show_info_circle(Circle* circle);
void take_info_circle(Circle* circle, int* column, int is_file, FILE* file);
