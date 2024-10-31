#pragma once
#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    KEYWORD,
    INT_LITERAL,
    SEMICOLON,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    EQUAL
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

Token check_string(char current, FILE* p_file);
Token check_int_lit(char current, FILE* p_file);
Token tokenize_brackets(char current);
Token tokenize_semicolon(char current);
Token* lexer(FILE* p_file, int* length);
void print_tokens(Token* tokens, int* length);
void free_tokens(Token* tokens, int length);
