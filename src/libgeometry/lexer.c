#include <ctype.h>
#include <libgeometry/geom_parser.h>
#include <libgeometry/lexer.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_error(int column, int status, int is_file, FILE* file)
{
    if (is_file == _FILE) {
        char temp[256];
        fseek(file, -column - 1, SEEK_CUR);
        fgets(temp, 255, file);
        printf("\n%s", temp);
    }

    for (int i = 0; i < column; i++) {
        putchar(' ');
    }

    printf(RED_COLOR("^") "\n");
    switch (status) {
    case ER_NAME:
        printf(RED_COLOR("Error") " at column %d: ", column);
        printf(RED_COLOR("expected 'circle' or 'triangle'") "\n");
        break;
    case ER_NOT_DOUBLE:
        printf(RED_COLOR("Error") " at column %d: ", column);
        printf(RED_COLOR("expected '<double>'") "\n");
        break;
    case ER_BACKSLASH:
        printf(RED_COLOR("Error") " at column %d: ", column);
        printf(RED_COLOR("expected ')'") "\n");
        break;
    case ER_UNEXPECT_TOKEN:
        printf(RED_COLOR("Error") " at column %d: ", column);
        printf(RED_COLOR("unexpected token") "\n");
        break;
    case ER_EXPECT_COMMA:
        printf(RED_COLOR("Error") " at column %d: ", column);
        printf(RED_COLOR("expected ','") "\n");
        break;
    case ER_UNEXPECT_COMMA:
        printf(RED_COLOR("Error") " at column %d: ", column);
        printf(RED_COLOR("unexpected ','") "\n");
    }
}

void to_lower_string(char* string)
{
    while (*string != '\0') {
        *string = tolower(*string);
        string++;
    }
}

void del_space(int* column, FILE* file)
{
    char ch;
    while ((ch = getc(file)) == ' ') {
        *column += 1;
    }
    ungetc(ch, file);
}

double get_number(int* column, int is_file, FILE* file)
{
    char temp[25] = {0};
    char ch;
    int point_count = 0;
    int i = 0;
    int minus_count = 0;

    del_space(column, file);

    while ((ch = getc(file)) != ' ') {
        temp[i] = ch;

        if (temp[i] == '.') {
            point_count++;
            if (point_count > 1) {
                if (is_file == _FILE)
                    print_error(*column + i + 1, ER_NOT_DOUBLE, _FILE, file);
                else
                    print_error(*column + i + 1, ER_NOT_DOUBLE, NOT_FILE, file);
                exit(EXIT_FAILURE);
            }
        }

        if (temp[i] == '-') {
            minus_count++;
            if (minus_count > 1) {
                if (is_file == _FILE)
                    print_error(*column + i + 1, ER_NOT_DOUBLE, _FILE, file);
                else
                    print_error(*column + i + 1, ER_NOT_DOUBLE, NOT_FILE, file);
                exit(EXIT_FAILURE);
            }
        }

        if (temp[i] == ')') {
            ungetc(temp[i], file);
            break;
        }

        if (temp[i] == ',') {
            ungetc(temp[i], file);
            break;
        }

        if (temp[i] == '(') {
            if (is_file == _FILE)
                print_error(*column + i, ER_BACKSLASH, _FILE, file);
            else
                print_error(*column + i, ER_BACKSLASH, NOT_FILE, file);
            exit(EXIT_FAILURE);
        }

        if (!isdigit(temp[i]) && temp[i] != '.' && temp[i] != '-') {
            if (is_file == _FILE)
                print_error(*column + i, ER_NOT_DOUBLE, _FILE, file);
            else
                print_error(*column + i, ER_NOT_DOUBLE, NOT_FILE, file);
            exit(EXIT_FAILURE);
        }

        i++;
    }
    if (ch == ' ')
        i++;
    del_space(column, file);
    *column += i;
    return strtod(temp, NULL);
}

bool expect(char expect, int* column, int status, int is_file, FILE* file)
{
    char ch;
    if ((ch = getc(file)) == expect) {
        *column += 1;
        return true;
    } else {
        if (is_file == _FILE)
            print_error(*column - 1, status, _FILE, file);
        else
            print_error(*column - 1, status, NOT_FILE, file);
        exit(EXIT_FAILURE);
    }
}

bool unexpect(char unexpect, int* column, int status, int is_file, FILE* file)
{
    char ch;
    if ((ch = getc(file)) == unexpect) {
        if (is_file == _FILE)
            print_error(*column, status, _FILE, file);
        else
            print_error(*column, status, NOT_FILE, file);
        exit(EXIT_FAILURE);
    }
    ungetc(ch, file);
    return true;
}

void get_point(Point* point, int* column, int is_file, FILE* file)
{
    if (is_file == _FILE) {
        point->x = get_number(column, _FILE, file);
        unexpect(',', column, ER_UNEXPECT_COMMA, _FILE, file);

        point->y = get_number(column, _FILE, file);
    } else {
        point->x = get_number(column, NOT_FILE, file);
        unexpect(',', column, ER_UNEXPECT_COMMA, NOT_FILE, file);

        point->y = get_number(column, NOT_FILE, file);
    }
}

void end_of_line(int* column, int is_file, FILE* file)
{
    char ch;
    while ((ch = getc(file)) != '\n' && ch != EOF) {
        if (ch != ' ') {
            if (is_file == _FILE)
                print_error(*column, ER_UNEXPECT_TOKEN, _FILE, file);
            else
                print_error(*column, ER_UNEXPECT_TOKEN, NOT_FILE, file);
            exit(EXIT_FAILURE);
        }
        *column += 1;
    }
}