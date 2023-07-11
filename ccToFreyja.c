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
    Node *node = expr();

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    
    gen(node);
    
    printf("    pop rax\n");
    printf("    ret\n");
    
    return 0;
}