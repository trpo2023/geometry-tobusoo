#include <ctype.h>
#include <math.h>
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
    _FILE,
    NOT_FILE,
    ER_NAME,
    ER_NOT_DOUBLE,
    ER_BACK_BRACE,
    ER_UNEXPECT_TOKEN,
    ER_EXPECT_COMMA,
    ER_UNEXPECT_COMMA,
};

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
    printf("\e[1;31m^\e[0m\n");
    switch (status) {
    case ER_NAME:
        printf("\e[1;31mError\e[0m at column %d: \e[1;31mexpected "
               "'circle'\e[0m\n",
               column);
        break;
    case ER_NOT_DOUBLE:
        printf("\e[1;31mError\e[0m at column %d: \e[1;31mexpected "
               "'<double>'\e[0m\n",
               column);
        break;
    case ER_BACK_BRACE:
        printf("\e[1;31mError\e[0m at column %d: \e[1;31mexpected ')'\e[0m\n",
               column);
        break;
    case ER_UNEXPECT_TOKEN:
        printf("\e[1;31mError\e[0m at column %d: \e[1;31munexpected "
               "token\e[0m\n",
               column);
        break;
    case ER_EXPECT_COMMA:
        printf("\e[1;31mError\e[0m at column %d: \e[1;31mexpected ','\e[0m\n",
               column);
        break;
    case ER_UNEXPECT_COMMA:
        printf("\e[1;31mError\e[0m at column %d: \e[1;31munexpected ','\e[0m\n",
               column);
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
        continue;
    }
    if (ch != ' ')
        ungetc(ch, file);
}

double get_number(int* column, int is_file, FILE* file)
{
    char temp[25];
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
            i++;
            break;
        }

        if (temp[i] == ',') {
            ungetc(temp[i], file);
            break;
        }

        if (temp[i] == '(') {
            i++;
            if (is_file == _FILE)
                print_error(*column + i, ER_BACK_BRACE, _FILE, file);
            else
                print_error(*column + i, ER_BACK_BRACE, NOT_FILE, file);
            exit(EXIT_FAILURE);
        }

        if (!isdigit(temp[i]) && temp[i] != '.' && temp[i] != '-') {
            i++;
            if (is_file == _FILE)
                print_error(*column + i, ER_NOT_DOUBLE, _FILE, file);
            else
                print_error(*column + i, ER_NOT_DOUBLE, NOT_FILE, file);
            exit(EXIT_FAILURE);
        }

        i++;
    }
    del_space(column, file);
    *column += i + 1;
    char* eptr;
    return strtod(temp, &eptr);
}

bool expect(char expect, int* column, int status, int is_file, FILE* file)
{
    char ch;
    if ((ch = getc(file)) == expect) {
        return true;
    } else {
        if (is_file == _FILE)
            print_error(*column, status, _FILE, file);
        else
            print_error(*column, status, NOT_FILE, file);
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

void take_info_circle(Circle* circle, int* column, int is_file, FILE* file)
{
    if (is_file == _FILE) {
        get_point(&circle->point, column, _FILE, file);
        expect(',', column, ER_EXPECT_COMMA, _FILE, file);

        circle->raduis = get_number(column, _FILE, file);

        expect(')', column, ER_BACK_BRACE, _FILE, file);

        end_of_line(column, _FILE, file);
    } else {
        get_point(&circle->point, column, NOT_FILE, file);
        expect(',', column, ER_EXPECT_COMMA, NOT_FILE, file);

        circle->raduis = get_number(column, NOT_FILE, file);

        expect(')', column, ER_BACK_BRACE, NOT_FILE, file);

        end_of_line(column, NOT_FILE, file);
    }
    circle->perimeter = 2 * M_PI * circle->raduis;
    circle->area = M_PI * circle->raduis * circle->raduis;
}

void show_info_circle(Circle* circle)
{
    printf("circle(%.2f %.2f, %.2f)\n",
           circle->point.x,
           circle->point.y,
           circle->raduis);
    printf("\tarea = %.4f\n", circle->area);
    printf("\tperimeter = %.4f\n", circle->perimeter);
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
                if (strcmp(geom, "circle") == 0) {
                    Circle circle;
                    take_info_circle(&circle, &column, NOT_FILE, stdin);
                    printf("\nYou have entered: \n");
                    show_info_circle(&circle);
                    break;
                } else {
                    print_error(0, ER_NAME, NOT_FILE, stdin);
                    exit(EXIT_FAILURE);
                }
            }

            if (ch == ')') {
                print_error(column, ER_BACK_BRACE, NOT_FILE, stdin);
                exit(EXIT_FAILURE);
            }

            geom[column++] = ch;

        } while ((ch = getc(stdin)) != '\n');
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
                if (strcmp(geom, "circle") == 0) {
                    Circle circle;
                    take_info_circle(&circle, &column, _FILE, file);
                    printf("\nYou have entered: \n");
                    show_info_circle(&circle);
                    break;
                } else {
                    print_error(0, ER_NAME, _FILE, file);
                    exit(EXIT_FAILURE);
                }
            }

            if (ch == ')') {
                print_error(column, ER_BACK_BRACE, _FILE, file);
                exit(EXIT_FAILURE);
            }

            geom[column++] = ch;

        } while ((ch = getc(file)) != '\n');
    }
}

int main(int argc, char* argv[])
{
    FILE* file = NULL;
    if (argc < 2)
        parser_stdin(stdin);
    else if (argc == 2) {
        if ((file = fopen(argv[1], "r")) == NULL) {
            printf("\e[1;31mError\e[0m: can't open file \e[1;35m\"%s\"\e[0m\n",
                   argv[1]);
            exit(EXIT_FAILURE);
        } else {
            parser_file(file);
            fclose(file);
        }
    } else {
        printf("\e[1;35mUsage\e[0m: %s <filename>\n", argv[0]);
    }

    return 0;
}