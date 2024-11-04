#include "codegen.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_SIZE                  4
#define INITIAL_VARIABLE_CAPACITY 8
#define MAX_OFFSET_STRING_LENGTH  20

typedef struct {
    char* name;
    char* value;
    int stack_offset;
} Variable;

typedef struct {
    Variable* variables;
    int variables_capacity;
    int stack_length;
    int index;
} Generator;

char* gen_expr(const NodeExpr expr, Generator* gen) {
    switch (expr.type) {
    case NODE_EXPR_INT_LIT:
        return expr.value.int_lit.value;

    case NODE_EXPR_IDENTIFIER: {
        for (int i = 0; i < gen->index; i++) {
            // While this code works well for now, doing stuff like
            // declaring multiple variables with the same name could cause
            // undefined behavior.
            if (strcmp(gen->variables[i].name, expr.ident.identifier.value) == 0) {
                char* res = malloc(MAX_OFFSET_STRING_LENGTH);
                if (!res) {
                    printf("Codegen: Memory allocation failed\n");
                    exit(1);
                }
                snprintf(res, MAX_OFFSET_STRING_LENGTH, "[rbp-%d]", gen->variables[i].stack_offset);
                return res;
            }
        }
        printf("Codegen: Undefined variable referenced\n");
        exit(1);
    }

    default:
        printf("Codegen: Unsupported expression type\n");
        exit(1);
    }
}

void gen_stmt(const NodeStmt stmt, FILE* output, Generator* gen) {
    char* expr;

    switch (stmt.type) {
    case NODE_STMT_EXIT:
        expr = gen_expr(stmt.exit_stmt.expr, gen);
        fprintf(output, "    mov rax, 60\n");
        fprintf(output, "    mov rdi, %s\n", expr);
        fprintf(output, "    syscall\n");
        free(expr);
        break;

    case NODE_STMT_VAR:
        expr = gen_expr(stmt.var_stmt.value, gen);
        if (strcmp("int", stmt.var_stmt.type) == 0) {
            if (gen->index >= gen->variables_capacity) {
                int new_capacity = gen->variables_capacity * 2;
                Variable* temp = realloc(gen->variables, sizeof(Variable) * new_capacity);
                if (!temp) {
                    printf("Codegen: Failed to reallocate memory for variables\n");
                    exit(1);
                }
                gen->variables = temp;
                gen->variables_capacity = new_capacity;
            }

            Variable var = {
                .name = stmt.var_stmt.identifier, .value = expr, .stack_offset = gen->stack_length + INT_SIZE};

            fprintf(output, "    mov dword [rbp-%d], %s\n", gen->stack_length + INT_SIZE, expr);

            gen->variables[gen->index++] = var;
            gen->stack_length += INT_SIZE;
        } else {
            printf("Codegen: Unsupported variable type\n");
            exit(1);
        }
        break;

    default:
        printf("Codegen: Unsupported statement type\n");
        exit(1);
    }
}

void gen_prog(const NodeProg* prog, FILE* output, Generator* gen) {
    for (int i = 0; i < prog->length; i++) {
        gen_stmt(prog->stmts[i], output, gen);
    }
}

int codegen(NodeProg* prog, char* filename) {
    FILE* output = fopen(filename, "w");
    if (!output) {
        printf("Codegen: Couldn't open output file\n");
        return 0;
    }

    Generator gen;
    gen.variables = malloc(sizeof(Variable) * INITIAL_VARIABLE_CAPACITY);
    if (!gen.variables) {
        printf("Codegen: Failed to allocate memory for variables\n");
        fclose(output);
        return 0;
    }
    gen.variables_capacity = INITIAL_VARIABLE_CAPACITY;
    gen.stack_length = 0;
    gen.index = 0;

    fprintf(output, "global _start\n\n");
    fprintf(output, "_start:\n");
    fprintf(output, "    push rbp\n");
    fprintf(output, "    mov rbp, rsp\n");

    gen_prog(prog, output, &gen);

    fprintf(output, "    mov rax, 60\n");
    fprintf(output, "    mov rdi, 0\n");
    fprintf(output, "    syscall\n");

    fclose(output);
    free(gen.variables);

    return 1;
}
