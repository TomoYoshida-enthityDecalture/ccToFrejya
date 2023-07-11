#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ccToFreyja.h"

Token *current;

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

bool consume(char *op) {
    if (current->type != TK_RESERVED || strlen(op) != current->len
    || memcmp(current->str, op, current->len)) return false;
    current = current->next;
    return true;
}

void expected(char *op) {
    if (current->type != TK_RESERVED || strlen(op) != current->len
    || memcmp(current->str, op, current->len)) 
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
Token *new_token(Tokentype type, Token *cur, char *str, int len) {
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    new->str = str;
    new->len = len;
    new->next = NULL;
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
        } else if (strncmp(p, "==", 2) == 0 || strncmp(p, "!=", 2) == 0 
        || strncmp(p, ">=", 2) == 0 || strncmp(p, "<=", 2) == 0) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
        } else if (*p == '>' || *p == '<' || *p == '+' 
        || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
            cur = new_token(TK_RESERVED, cur, p++, 1);
        } else if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 1);
            cur->num = strtol(p, &p, 10);
        } else {
            error_at(current->str, "Cannot tokenize");
        }
    }

    new_token(TK_EOF, cur, p, 1);
    return head.next;
}
