#pragma once
#include <libgeometry/geom_parser.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void print_error(int column, int status, int is_file, FILE* file);
void to_lower_string(char* string);
void del_space(int* column, FILE* file);
double get_number(int* column, int is_file, FILE* file);
bool expect(char expect, int* column, int status, int is_file, FILE* file);
bool unexpect(char unexpect, int* column, int status, int is_file, FILE* file);
void end_of_line(int* column, int is_file, FILE* file);
void get_point(Point* point, int* column, int is_file, FILE* file);