#include <stdio.h>

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

int main(void) {
    FILE* p_file;
    char current;
    Token* tokens;

    p_file = fopen("test.nl", "r");

    current = fgetc(p_file);
    while (current != EOF) {
        printf("%c", current);
        current = fgetc(p_file);
    }
    fclose(p_file);

    return 0;
}
