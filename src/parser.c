#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    Token* tokens;
    int length;
    int index;
} Parser;

// Helper functions.
// Implemented only for the parser and not for lexer
// because doing everything manually for the parser
// is insanely hard, unlike the lexer.
Token peek(Parser* parser, int offset) {
    if (parser->index + offset >= parser->length) {
        Token temp = {0};
        return temp;
    }

    return parser->tokens[parser->index + offset];
}

Token consume(Parser* parser) { return parser->tokens[parser->index++]; }

NodeExpr* parse_expr(Parser* parser) {
    Token token = peek(parser, 0);
    if (!token.value)
        return NULL;

    NodeExpr* expr = malloc(sizeof(NodeExpr));
    if (!expr) {
        printf("Parser: Failed to allocate memory for expression\n");
        exit(1);
    }

    if (token.type == INT_LITERAL) {
        expr->type = NODE_EXPR_INT_LIT;
        expr->value.int_lit = consume(parser);
        return expr;
    }

    if (token.type == IDENTIFIER) {
        expr->type = NODE_EXPR_IDENTIFIER;
        expr->ident.identifier = consume(parser);
        return expr;
    }

    free(expr);
    return NULL;
}

int parse_stmt_exit(Parser* parser, NodeStmt* stmt) {
    if (peek(parser, 1).type != OPEN_BRACKET)
        return 0;

    consume(parser); // KEYWORD token. (exit)
    consume(parser); // OPEN_BRACKET token.

    NodeExpr* expr = parse_expr(parser);
    if (!expr) {
        printf("Parser: Invalid expression in the exit statement.\n");
        free(stmt);
        exit(1);
    }

    stmt->type = NODE_STMT_EXIT;
    stmt->exit_stmt.expr = *expr;
    free(expr);

    if (peek(parser, 0).type != CLOSE_BRACKET) {
        printf("Parser: Expected ')'\n");
        free(stmt);
        exit(1);
    }
    consume(parser);

    if (peek(parser, 0).type != SEMICOLON) {
        printf("Parser: Expected ';'\n");
        free(stmt);
        exit(1);
    }
    consume(parser);

    return 1;
}

int parse_stmt_variable(Parser* parser, NodeStmt* stmt) {
    if (peek(parser, 1).type != IDENTIFIER) {
        printf("Parser: Expected an identifier after the type.\n");
        free(stmt);
        exit(1);
    }

    stmt->type = NODE_STMT_VAR;
    stmt->var_stmt.type = consume(parser).value;
    stmt->var_stmt.identifier = consume(parser).value;

    if (peek(parser, 0).type != EQUAL) {
        printf("Parser: Expected '='\n");
        free(stmt);
        exit(1);
    }
    consume(parser);

    NodeExpr* expr = parse_expr(parser);
    if (strcmp(stmt->var_stmt.type, "int") == 0) {
        if (expr && expr->type == NODE_EXPR_INT_LIT) {
            stmt->var_stmt.value = *expr;
        } else {
            printf("Parser: Unexpected type.\n");
            free(stmt);
            free(expr);
            exit(1);
        }
    }

    if (peek(parser, 0).type != SEMICOLON) {
        printf("Parser: Expected ';'\n");
        free(stmt);
        free(expr);
        exit(1);
    }
    consume(parser);
    free(expr);

    return 1;
}

NodeStmt* parse_stmt(Parser* parser) {
    Token token = peek(parser, 0);
    if (!token.value)
        return NULL;

    NodeStmt* stmt = malloc(sizeof(NodeStmt));
    if (!stmt) {
        printf("Parser: Failed to allocate memory for a statement node.\n");
        exit(1);
    }

    if (token.type == KEYWORD) {
        for (int i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
            if (strcmp(token.value, types[i]) == 0) {
                if (parse_stmt_variable(parser, stmt))
                    return stmt;
            }
        }
        if (strcmp(token.value, "exit") == 0) {
            if (parse_stmt_exit(parser, stmt))
                return stmt;
        }
    }

    free(stmt);
    return NULL;
}

NodeProg* parse_prog(Parser* parser) {
    NodeProg* prog = malloc(sizeof(NodeProg));
    if (!prog) {
        printf("Parser: Failed to allocate memory for the program node.\n");
        exit(1);
    }

    prog->capacity = 8;
    prog->length = 0;
    prog->stmts = malloc(sizeof(NodeStmt) * prog->capacity);
    if (!prog->stmts) {
        printf("Parser: Failed to allocate memory for the statements array in the program node.\n");
        free(prog);
        exit(1);
    }

    while (parser->index < parser->length) {
        NodeStmt* stmt = parse_stmt(parser);
        if (!stmt) {
            printf("Parser: Invalid statement.\n");
            free(stmt);
            free(prog->stmts);
            free(prog);
            exit(1);
        }

        if (prog->length >= prog->capacity) {
            prog->capacity *= 2;
            NodeStmt* temp = realloc(prog->stmts, sizeof(NodeStmt) * prog->capacity);
            if (!temp) {
                printf("Parser: Failed to reallocate memory for the statements array in the program node.\n");

                free(stmt);
                free(prog->stmts);
                free(prog);

                exit(1);
            }
            prog->stmts = temp;
        }

        // Add the statement node to our array.
        prog->stmts[prog->length++] = *stmt;
        free(stmt);
    }

    return prog;
}

void token_print(Token token) { printf("%s\n", token.value); }

void print_expr(NodeExpr* expr) {
    switch (expr->type) {
    case NODE_EXPR_INT_LIT:
        printf("Int Literal: ");
        token_print(expr->value.int_lit);
        break;
    case NODE_EXPR_IDENTIFIER:
        printf("Variable: ");
        token_print(expr->ident.identifier);
        break;
    default:
        printf("Unknown Expr");
        break;
    }
}

void print_stmt(NodeStmt* stmt) {
    switch (stmt->type) {
    case NODE_STMT_EXIT:
        printf("Exit Statement:\n  ");
        print_expr(&stmt->exit_stmt.expr);
        break;
    case NODE_STMT_VAR:
        printf("Variable Statement:\n ");
        printf("Identifier: %s\n ", stmt->var_stmt.identifier);
        printf("Type: %s\n ", stmt->var_stmt.type);
        print_expr(&stmt->var_stmt.value);
        break;
    default:
        printf("%d\n", stmt->type);
        printf("Unknown Stmt");
        break;
    }
}

void print_program(NodeProg* program) {
    printf("Program with %d statements:\n", program->length);
    for (int i = 0; i < program->length; i++) {
        printf("Statement %d: ", i + 1);
        print_stmt(&program->stmts[i]);
        printf("\n");
    }
}

NodeProg* parse(Token* tokens, int* length) {
    Parser parser = {.index = 0, .length = *length, .tokens = tokens};

    NodeProg* prog = parse_prog(&parser);
    return prog;
}
