#include "codegen.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

#define INT_SIZE 4

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

Generator gen;

char* gen_expr(NodeExpr expr) {
    // To stop the compiler from screaming.
    char* res;

    switch (expr.type) {
    case NODE_EXPR_INT_LIT:
        return expr.value.int_lit.value;
    case NODE_EXPR_IDENTIFIER:
        for (int i = 0; i < gen.index; i++) {
            // While this code works well for now, doing stuff like
            // declaring multiple variables with the same name could cause
            // undefined behavior.
            if (strcmp(gen.variables[i].name, expr.ident.identifier.value) == 0) {
                // TODO: fix memory leak.
                asprintf(&res, "[rbp-%d]", gen.variables[i].stack_offset);
                return res;
            }
        }
        break;
    default:
        break;
    }

    return NULL;
}

void gen_stmt(NodeStmt stmt, FILE* output) {
    char* expr;

    switch (stmt.type) {
    case NODE_STMT_EXIT:
        expr = gen_expr(stmt.exit_stmt.expr);
        if (!expr) {
            printf("Codegen: Failed to generate an expression.\n");
            exit(1);
        }

        fprintf(output, "    mov rax, 60\n");
        fprintf(output, "    mov rdi, %s\n", expr);
        fprintf(output, "    syscall\n");
        break;
    case NODE_STMT_VAR:
        expr = gen_expr(stmt.var_stmt.value);
        if (!expr) {
            printf("Codegen: Failed to generate an expression.\n");
            exit(1);
        }

        if (strcmp("int", stmt.var_stmt.type) == 0) {
            // 4 bytes offset for an int.
            Variable var = {
                .name = stmt.var_stmt.identifier, .value = expr, .stack_offset = gen.stack_length + INT_SIZE};
            fprintf(output, "    mov dword [rbp-%d], %s\n", gen.stack_length + INT_SIZE, expr);
            gen.variables[gen.index] = var;
            gen.index++;
            gen.stack_length += INT_SIZE;
        }
        break;
    default:
        break;
    }
}

void gen_prog(NodeProg* prog, FILE* output) {
    for (int i = 0; i < prog->length; i++) {
        gen_stmt(prog->stmts[i], output);

        if (gen.index >= gen.variables_capacity) {
            gen.variables_capacity *= 2;
            Variable* temp = realloc(gen.variables, sizeof(Variable) * gen.variables_capacity);
            if (!temp) {
                printf("Codegen: Failed to reallocate memory for the variables array.\n");
                // For some reason freeing `gen.variables` causes a double free.
                exit(1);
            }
            gen.variables = temp;
        }
    }
}

int codegen(NodeProg* prog, char* filename) {
    FILE* output = fopen(filename, "w");
    if (!output) {
        printf("Codegen: Couldn't open a new file.\n");
        exit(1);
    }

    gen.variables = malloc(sizeof(Variable) * 8);
    gen.variables_capacity = 8;
    gen.stack_length = 0;
    gen.index = 0;

    fprintf(output, "global _start\n\n");
    fprintf(output, "_start:\n");
    fprintf(output, "    push rbp\n");
    fprintf(output, "    mov rbp, rsp\n");
    gen_prog(prog, output);

    // Default behavior. (probably)
    fprintf(output, "    mov rax, 60\n");
    fprintf(output, "    mov rdi, 0\n");
    fprintf(output, "    syscall\n");

    fclose(output);
    free(gen.variables);

    return 1;
}
