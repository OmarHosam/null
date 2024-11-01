#include "codegen.h"
#include "parser.h"
#include <stdlib.h>

char* gen_expr(NodeExpr expr) {
    switch (expr.type) {
    case NODE_EXPR_INT_LIT:
        return expr.value.int_lit.value;
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
    default:
        break;
    }
}

void gen_prog(NodeProg* prog, FILE* output) {
    for (int i = 0; i < prog->length; i++) {
        gen_stmt(prog->stmts[i], output);
    }
}

int codegen(NodeProg* prog, char* filename) {
    FILE* output = fopen(filename, "w");
    if (!output) {
        printf("Codegen: Couldn't open a new file.\n");
        exit(1);
    }

    fprintf(output, "global _start\n\n");
    fprintf(output, "_start:\n");
    gen_prog(prog, output);

    // Default behavior. (probably)
    fprintf(output, "    mov rax, 60\n");
    fprintf(output, "    mov rdi, 0\n");
    fprintf(output, "    syscall\n");

    fclose(output);

    return 1;
}
