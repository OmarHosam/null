#pragma once

typedef enum {
    KEYWORD,
    INT_LITERAL,
    SEMICOLON,
    OPEN_BRACKET,
    CLOSE_BRACKET
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;
