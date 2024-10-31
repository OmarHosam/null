#include "lexer.h"
#include <stdlib.h>

Token check_string(char current, FILE* p_file) {
    // TODO: fix buffer overflow.
    char buffer[16];
    int i = 0;
    Token token = {0};

    while (current != EOF && isalpha(current)) {
        buffer[i++] = current;
        current = fgetc(p_file);
    }
    buffer[i] = '\0';

    // Not so efficient, but it get's the job done for now.
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(buffer, keywords[i]) == 0) {
            token.type = KEYWORD;
            token.value = strdup(buffer);

            ungetc(current, p_file); // Move back one step.
            return token;
        }
    }

    for (int i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
        if (strcmp(buffer, types[i]) == 0) {
            token.type = KEYWORD;
            token.value = strdup(buffer);

            ungetc(current, p_file); // Move back one step.
            return token;
        }
    }

    token.type = IDENTIFIER;
    token.value = strdup(buffer);

    ungetc(current, p_file); // Move back one step.
    return token;
}

Token check_int_lit(char current, FILE* p_file) {
    // TODO: fix buffer overflow.
    char buffer[8];
    int i = 0;
    Token token = {INT_LITERAL, NULL};

    while (current != EOF && isdigit(current)) {
        buffer[i++] = current;
        current = fgetc(p_file);
    }
    buffer[i] = '\0';

    // Because the buffer is getting freed after the function exits,
    // it's better to duplicate the string. That way, we can avoid undefined
    // behavior.
    token.value = strdup(buffer);
    ungetc(current, p_file); // Move back one step.
    return token;
}

Token tokenize_brackets(char current) {
    Token l_token;
    if (current == '(') {
        l_token.type = OPEN_BRACKET;
        l_token.value = "(";
    }

    if (current == ')') {
        l_token.type = CLOSE_BRACKET;
        l_token.value = ")";
    }

    return l_token;
}

Token tokenize_semicolon(char current) {
    Token l_token;

    l_token.type = SEMICOLON;
    l_token.value = ";";

    return l_token;
}

Token tokenize_equal(char current) {
    Token l_token;

    l_token.type = EQUAL;
    l_token.value = "=";

    return l_token;
}

// `int length` will return the length for the tokens.
Token* lexer(FILE* p_file, int* length) {
    char current;
    int capacity = 8;
    Token* tokens = malloc(sizeof(Token) * capacity);

    int token_index = 0;
    current = fgetc(p_file);

    while (current != EOF) {
        Token curr_token;

        // Thanks ChatGPT for the refactoring!
        if (isalpha(current)) {
            curr_token = check_string(current, p_file);
        } else if (isdigit(current)) {
            curr_token = check_int_lit(current, p_file);
        } else if (current == ')' || current == '(') {
            curr_token = tokenize_brackets(current);
        } else if (current == ';') {
            curr_token = tokenize_semicolon(current);
        } else if (current == '=') {
            curr_token = tokenize_equal(current);
        } else {
            current = fgetc(p_file);
            continue;
        }

        if (token_index >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, sizeof(Token) * capacity);
            if (tokens == NULL) {
                printf("Lexer Error: failed to reallocate more memory for the tokens.\n");
                exit(1);
            }
        }

        tokens[token_index++] = curr_token;
        current = fgetc(p_file);
    }

    *length = token_index;

    return tokens;
}

void print_token(Token token) {
    printf("Token value: '%s'\t", token.value);
    switch (token.type) {
    case (INT_LITERAL):
        printf("Token type: INT_LITERAL\n");
        break;
    case (KEYWORD):
        printf("Token type: KEYWORD\n");
        break;
    case (SEMICOLON):
        printf("Token type: SEMICOLON\n");
        break;
    case (OPEN_BRACKET):
        printf("Token type: OPEN_BRACKET\n");
        break;
    case (CLOSE_BRACKET):
        printf("Token type: CLOSE_BRACKET\n");
        break;
    case (EQUAL):
        printf("Token type: EQUAL\n");
        break;
    case (IDENTIFIER):
        printf("Token type: IDENTIFIER\n");
        break;
    }
}

void print_tokens(Token* tokens, int* length) {
    for (int i = 0; i < *length; i++) {
        print_token(tokens[i]);
    }
}

void free_tokens(Token* tokens, int length) {
    for (int i = 0; i < length; i++) {
        if (tokens[i].type == INT_LITERAL || tokens[i].type == KEYWORD) {
            free(tokens[i].value);
        }
    }

    free(tokens);
}
