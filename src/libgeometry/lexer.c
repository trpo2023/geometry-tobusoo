#include <ctype.h>
#include <stdlib.h>

#include <libgeometry/geom_parser.h>
#include <libgeometry/lexer.h>

void print_error(int column, int status, int is_file, FILE* file)
{
    if (is_file == _FILE) {
        char temp[256];
        fseek(file, -column - 1, SEEK_CUR);
        fgets(temp, 255, file);
        printf("\n%s\n", temp);
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
        break;
    case ER_NEGATIVE_R:
        printf(RED_COLOR("Error") " at column %d: ", column);
        printf(RED_COLOR("radius must be positive") "\n");
        break;
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

void count_char(char ch, char exp_ch, int* cnt, int* clmn, int is_file, FILE* f)
{
    if (ch == exp_ch)
        *cnt += 1;
    if (*cnt > 1) {
        print_error(*clmn + 1, ER_NOT_DOUBLE, is_file, f);
        exit(EXIT_FAILURE);
    }
}

bool unexpect_char(char ch, char e_ch, FILE* file)
{
    if (ch == e_ch) {
        ungetc(ch, file);
        return true;
    }
    return false;
}

double get_number(int* column, int is_file, FILE* file)
{
    char temp[25] = {0};
    char ch;
    int point_count = 0, minus_count = 0, i = 0;

    del_space(column, file);

    while ((ch = getc(file)) != ' ') {
        temp[i] = ch;

        count_char(temp[i], '.', &point_count, column + i, is_file, file);
        count_char(temp[i], '-', &minus_count, column + i, is_file, file);

        if (unexpect_char(temp[i], ')', file) == true)
            break;

        if (unexpect_char(temp[i], ',', file) == true)
            break;

        if (temp[i] == '(') {
            print_error(*column + i, ER_BACKSLASH, is_file, file);
            exit(EXIT_FAILURE);
        }

        if (!isdigit(temp[i]) && temp[i] != '.' && temp[i] != '-') {
            print_error(*column + i, ER_NOT_DOUBLE, is_file, file);
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
            print_error(*column, status, _FILE, file);
        else
            print_error(*column - 1, status, NOT_FILE, file);
        exit(EXIT_FAILURE);
    }
}

bool unexpect(char unexpect, int* column, int status, int is_file, FILE* file)
{
    char ch;
    if ((ch = getc(file)) == unexpect) {
        print_error(*column, status, is_file, file);
        exit(EXIT_FAILURE);
    }
    ungetc(ch, file);
    return true;
}

void get_point(Point* point, int* column, int is_file, FILE* file)
{
    point->x = get_number(column, is_file, file);
    unexpect(',', column, ER_UNEXPECT_COMMA, is_file, file);

    point->y = get_number(column, is_file, file);
}

void end_of_line(int* column, int is_file, FILE* file)
{
    char ch;
    while ((ch = getc(file)) != '\n' && ch != EOF) {
        if (ch != ' ') {
            print_error(*column, ER_UNEXPECT_TOKEN, is_file, file);
            exit(EXIT_FAILURE);
        }
        *column += 1;
    }
}