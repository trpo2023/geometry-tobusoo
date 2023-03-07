#include <libgeometry/geom_parser.h>
#include <libgeometry/lexer.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* file = NULL;
    if (argc < 2)
        parser_stdin(stdin);
    else if (argc == 2) {
        if ((file = fopen(argv[1], "r")) == NULL) {
            printf(RED_COLOR("Error") ": can't open file \e[1;35m\"%s\"\e[0m\n",
                   argv[1]);
            exit(EXIT_FAILURE);
        } else {
            parser_file(file);
            fclose(file);
        }
    } else {
        printf(MAGENTA_COLOR("Usage") ": %s <filename>\n", argv[0]);
    }

    return 0;
}