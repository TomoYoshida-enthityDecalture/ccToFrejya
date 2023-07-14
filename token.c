#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ccToFreyja.h"

Node *code[100];

//generate new node
Node *new_node(Nodetype type, Node *lhs, Node *rhs) {
    Node *new = calloc(1, sizeof(Node));
    new->type = type;
    new->lhs = lhs;
    new->rhs = rhs;
    
    return new;
}

//generate leaf num
Node *new_node_num(int val) {
    Node *new = calloc(1, sizeof(Node));
    new->type = ND_NUM;
    new->val = val;

    return new;
}

Node *new_node_ident(char *val) {
    Node *new = calloc(1, sizeof(Node));
    new->type = ND_LVAR;
    new->offset = (*val - 'a' + 1) * 8;
    return new;
}

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relation();
Node *add();
Node *mul();
Node *unary();
Node *primary();

void program() {
    int i = 0;
    while (!at_eof()) {
        code[i++] = stmt();
    }
    code[i] = NULL;
}

Node *stmt() {
    Node *node = expr();
    expected(";");
    return node;
}

Node *expr() {
    return assign();
}

Node *assign() {
    Node *node = equality();

    if (consume("=")) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}

Node *equality() {
    Node *node = relation();

    while (1) {
        if (consume("==")) node = new_node(ND_EQL, node, relation());
        else if (consume("!=")) node = new_node(ND_NEQ, node, relation());
        else return node;
    }
}

Node *relation() {
    Node *node = add();

    while (1) {
        if (consume("<=")) node = new_node(ND_LEQ, node, add());
        else if (consume(">=")) node = new_node(ND_GEQ, node, add());
        else if (consume("<")) node = new_node(ND_LE, node, add());
        else if (consume(">")) node = new_node(ND_GE, node, add());
        else return node;
    }
}

Node *add() {
    Node *node = mul();

    while (1) {
        if (consume("+")) node = new_node(ND_ADD, node, mul());
        else if (consume("-")) node = new_node(ND_SUB, node, mul());
        else return node;
    }
}

Node *mul() {
    Node *node = unary();
    
    while (1) {
        if (consume("*")) node = new_node(ND_MUL, node, unary());
        else if (consume("/")) node = new_node(ND_DIV, node, unary());
        else return node;
    }
}

Node *unary() {
    if (consume("+")) {
        return primary();
    } else if (consume("-")) {
        return new_node(ND_SUB, new_node_num(0), primary());
    } else return primary();
}

Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expected(")");
        return node;
    } else if (consume_ident()) {
        return new_node_ident(expected_ident()); 
    } else {
        return new_node_num(expected_num());
    }
}
