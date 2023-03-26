#pragma once
#include <stdio.h>

typedef struct point {
    double x;
    double y;
} Point;

typedef struct circle {
    Point point;
    double radius;
    double perimeter;
    double area;
    char* name;
} Circle;

typedef struct triangle {
    Point p1;
    Point p2;
    Point p3;
    Point p4;
    double perimeter;
    double area;
    char* name;
} Triangle;

void show_info_triangle(Triangle* tr);
void take_info_triangle(Triangle* tr, int* column, int is_file, FILE* file);
void parser(FILE* file, int is_file);
void show_info_circle(Circle* circle);
void take_info_circle(Circle* circle, int* column, int is_file, FILE* file);
