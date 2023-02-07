#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 25

int main()
{
    char geom[NAME_SIZE];
    char ch;
    int column = 0;

    puts("Enter a geometric shape (or q for exit):");

    while ((ch = getchar()) != '\n' && ch != 'q') {
        do {
            if (ch == '(' || ch == ' ') {
                if (strcmp(geom, "circle") == 0) {
                    puts(geom);
                    break;
                } else {
                    puts("Error: expected 'circle'");
                    exit(EXIT_FAILURE);
                }
            }
            geom[column++] = ch;

        } while ((ch = getchar()) != '\n');
    }

    return 0;
}