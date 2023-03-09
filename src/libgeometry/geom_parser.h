#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct point {
    double x;
    double y;
} Point;

typedef struct circle {
    Point point;
    double radius;
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

void show_info_triangle(Triangle* tr);
void take_info_triangle(Triangle* tr, int* column, int is_file, FILE* file);
void parser_file(FILE* file);
void parser_stdin(FILE* stdin);
void show_info_circle(Circle* circle);
void take_info_circle(Circle* circle, int* column, int is_file, FILE* file);
