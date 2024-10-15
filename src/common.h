#pragma once

typedef enum {
    ROOT,
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
