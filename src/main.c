#include "common.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

void print_usage(char* loc) { printf("Usage: %s <filename.nl>\n", loc); }

int main(int argc, char** argv) {
    if (!argv[1]) {
        printf("Wrong usage!\n");
        print_usage(argv[0]);
        exit(1);
    }

    FILE* p_file;
    int length;

    p_file = fopen(argv[1], "r");
    if (!p_file) {
        printf("'%s' is not found!\n", argv[1]);
        print_usage(argv[0]);
        exit(1);
    }

    Token* tokens = lexer(p_file, &length);
    print_tokens(tokens, &length);

    tokens = parse(tokens, &length);

    // Cleanup.
    fclose(p_file);
    free_tokens(tokens, length);

    return 0;
}
