#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//type of Token
typedef enum {
    TK_RESERVED,
    TK_NUM,
    TK_EOF
} Tokentype;

typedef struct Token Token;

//Token as a type
struct Token {
    Tokentype type;
    Token *next;
    int num;
    char *str; 
};

//current token
Token *current;

//input program
char *user_input;

//error_at
void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ "); 
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume(char op) {
    if (current->type != TK_RESERVED || current->str[0] != op) return false;
    current = current->next;
    return true;
}

void expected(char op) {
    if (current->type != TK_RESERVED || current->str[0] != op) 
        error_at(current->str, "Not %c\n", op);
    current = current->next;
}

int expected_num() {
    if (current->type != TK_NUM) 
        error_at(current->str, "Not number\n");
    int val = current->num;
    current = current->next;
    return val;
}

bool at_eof() {
    return current->type == TK_EOF;
}

//generate a new token and connect to cur
Token *new_token(Tokentype type, Token *cur, char *str) {
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    new->str = str;
    cur->next = new;
    return new;
}

//tokenize input
Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            ++p;
        } else if (*p == '+' || *p == '-') {
            cur = new_token(TK_RESERVED, cur, p++);
        } else if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->num = strtol(p, &p, 10);
        } else {
            error_at(current->str, "Cannot tokenize");
        }
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int args, char **argv) {
    if (args != 2) {
        fprintf(stderr, "Incorrect number of argment.");
        return 1;
    }

    user_input = argv[1];
    current = tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    
    printf("    mov rax, %d\n", expected_num());
    
    while (!at_eof()) {
        if (consume('+')) {
            printf("    add rax, %d\n", expected_num());
            continue;
        }        
        expected('-');
        printf("    sub rax, %d\n", expected_num());
    }

    free(current);
    printf("    ret\n");
    
    return 0;
}