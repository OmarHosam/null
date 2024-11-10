#pragma once
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "keywords.h"
#include "types.h"

typedef enum {
    KEYWORD,
    INT_LITERAL,
    SEMICOLON,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    EQUAL,
    IDENTIFIER,
    ADDITION
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

Token* lexer(FILE* p_file, int* length);
void print_tokens(Token* tokens, int* length);
void free_tokens(Token* tokens, int length);
