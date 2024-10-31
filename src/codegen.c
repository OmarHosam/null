#include "codegen.h"
#include <stdlib.h>

int codegen(NodeProg* prog, char* filename) {
    FILE* output = fopen(filename, "w");
    if (!output) {
        printf("Codegen: Couldn't open a new file.\n");
        exit(1);
    }

    fprintf(output, "global _start\n\n");
    fprintf(output, "_start:\n\t");
    fprintf(output, "mov rax, 60\n\t");
    fprintf(output, "mov rdi, %s\n\t", prog->stmts[0].exit_stmt.expr.value.int_lit.value);
    fprintf(output, "syscall\n");

    fclose(output);

    return 1;
}
