#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ccToFreyja.h"
#include "codegen.h"

//generate code
void gen_lval(Node *node) {
    if (node->type !=ND_LVAR) 
        error("The left value is not varient.");

    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    printf("    push rax\n");
}

void gen(Node *node) {
    static int label_num = 0;
    switch (node->type) {
        case ND_NUM:
            printf("    push %d\n", node->val);
            return;
        case ND_LVAR:
            gen_lval(node);
            printf("    pop rax\n");
            printf("    mov rax, [rax]\n");
            printf("    push rax\n");
            return;
        case ND_ASSIGN:
            gen_lval(node->lhs);
            gen(node->rhs);
            printf("    pop rdi\n");
            printf("    pop rax\n");
            printf("    mov [rax], rdi\n");
            printf("    push rdi\n");
            return;
        case ND_FUNC:
            char *str = node->str;
            str[node->val] = '\0'; 
            printf("    call %s\n", str);
            return;
        case ND_BLOCK:
            if (node->lhs) gen(node->lhs);
            gen(node->rhs);
            printf("    pop rax\n");
            return;
        case ND_IF:
            gen(node->lhs);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            gen(node->rhs);
            return;
        case ND_ELSE:
            ++label_num;
            if (node->rhs) {
                printf("    je .Lelse%d\n", label_num);
                gen(node->lhs);
                printf("    jmp .Lend%d\n", label_num);
                printf(".Lelse%d:\n", label_num);
                gen(node->rhs);
                printf(".Lend%d:\n", label_num);
            } else {
                printf("    je .Lend%d\n", label_num);
                gen(node->lhs);
                printf(".Lend%d:\n", label_num);
            }
            return;
        case ND_WHILE:
            ++label_num;
            printf(".Lwbegin%d:\n", label_num);
            gen(node->lhs);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je .Lwend%d\n", label_num);
            gen(node->rhs);
            printf("    jmp .Lwbegin%d\n", label_num);
            printf(".Lwend%d:\n", label_num);
            return;
        case ND_FOR:
            ++label_num;
            gen(node->lhs);
            gen(node->rhs);
            return;
        case ND_FOR1:
            gen(node->lhs);
            printf(".Lfbegin%d:\n", label_num);
            gen(node->rhs);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je .Lfend%d\n", label_num);
            return;
        case ND_FOR2:
            gen(node->rhs);
            gen(node->lhs);
            printf("    jmp .Lfbegin%d\n", label_num);
            printf(".Lfend%d:\n", label_num);
            return;
        case ND_RETURN:
            gen(node->lhs);
            printf("    pop rax\n");
            printf("    mov rsp, rbp\n");
            printf("    pop rbp\n");
            printf("    ret\n");
            return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch (node->type) {
        case ND_ADD:
            printf("    add rax, rdi\n");
            break;
        case ND_SUB:
            printf("    sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("    imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("    cqo\n");
            printf("    idiv rdi\n");
            break;
        case ND_EQL:
            printf("    cmp rax, rdi\n");
            printf("    sete al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_NEQ:
            printf("    cmp rax, rdi\n");
            printf("    setne al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LEQ:
            printf("    cmp rax, rdi\n");
            printf("    setle al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_GEQ:
            printf("    cmp rdi, rax\n");
            printf("    setle al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LE:
            printf("    cmp rax, rdi\n");
            printf("    setl al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_GE:
            printf("    cmp rdi, rax\n");
            printf("    setl al\n");
            printf("    movzb rax, al\n");
            break;
    }

    printf("    push rax\n");
}
