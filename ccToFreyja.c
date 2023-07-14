#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ccToFreyja.h"

int main(int args, char **argv) {
    if (args != 2) {
        fprintf(stderr, "Incorrect number of argment.");
        return 1;
    }

    user_input = argv[1];
    current = tokenize(argv[1]);
    program();

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, 208\n");

    for (int i = 0; code[i]; ++i) {
        gen(code[i]);

        printf("    pop rax\n");   
    }
    
    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");

    return 0;
}