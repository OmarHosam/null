#pragma once
#include "common.h"

// https://keleshev.com/abstract-syntax-tree-an-example-in-c/ast.svg
typedef struct Node {
    TokenType type;
    struct Node* left;
    struct Node* right;
} Node;

Token* parse(Token* tokens, int* length);
