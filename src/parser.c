#include "parser.h"

struct NodeExprIntLit {
    Token int_lit;
};

// TODO: Refactor this code.
typedef struct NodeExpr NodeExpr;
typedef struct NodeStmt NodeStmt;

struct NodeExpr {
    enum {
        NODE_EXPR_INT_LIT
    } tag;
    union {
        struct NODE_EXPR_INT_LIT {
            Token int_lit;
        } AST_INT_LIT;
    } data;
};

struct NodeStmt {
    enum {
        NODE_EXIT
    } tag;
    union {
        struct NODE_EXIT {
            NodeExpr* expr;
        } AST_INT_LIT;
    } data;
};

struct NodeProg {
    NodeStmt* stmts;
    int size;
};

Token* parse(Token* tokens, int* length) { return tokens; }
