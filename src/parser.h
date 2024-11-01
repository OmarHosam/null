#pragma once
#include "lexer.h"
#include "types.h"

// https://keleshev.com/abstract-syntax-tree-an-example-in-c/ast.svg
typedef enum {
    NODE_EXPR_INT_LIT,
} NodeExprType;

typedef enum {
    NODE_STMT_EXIT,
    NODE_STMT_VAR,
} NodeStmtType;

typedef struct {
    NodeExprType type;
    union {
        struct {
            Token int_lit; // NODE_EXPR_INT_LIT
        } value;
    };
} NodeExpr;

typedef struct {
    NodeStmtType type;
    union {
        struct {
            NodeExpr expr;
        } exit_stmt;
        struct {
            char* type;
            char* identifier;
            NodeExpr value;
        } var_stmt;
    };
} NodeStmt;

typedef struct {
    NodeStmt* stmts;
    int length;
    int capacity;
} NodeProg;

NodeProg* parse(Token* tokens, int* length);
void print_program(NodeProg* program);
