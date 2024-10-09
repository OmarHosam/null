#include <stdio.h>
#include <stdlib.h>

typedef enum {
    EXIT,
    INT_LITERAL,
    SEMI_COLON,
    OPEN_BRACKET,
    CLOSE_BRACKET
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

void print_usage() {
    printf("Usage: null <filename.nl>\n");
}

int main(int argc, char** argv) {
    if (!argv[1]) {
        printf("Wrong usage!\n");
        print_usage();
        exit(1);
    }

    FILE* p_file;
    char current;
    Token* tokens;

    p_file = fopen(argv[1], "r");
    if (!p_file) {
        printf("This file is not found!\n");
        print_usage();
        exit(1);
    }

    current = fgetc(p_file);
    while (current != EOF) {
        printf("%c", current);
        current = fgetc(p_file);
    }
    fclose(p_file);

    return 0;
}
