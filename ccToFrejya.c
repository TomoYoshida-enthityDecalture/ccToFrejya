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
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
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

//ll tree Node type
typedef enum {
    ND_NUM,
    ND_ADD, 
    ND_SUB,
    ND_MUL,
    ND_DIV
} Nodetype;

typedef struct Node Node;

//ll tree Node
struct Node {
    Nodetype type;
    Node *lhs;
    Node *rhs;
    int val;
};

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

Node *expr();
Node *mul();
Node *primary();

Node *expr() {
    Node *node = mul();

    while (1) {
        if (consume('+')) node = new_node(ND_ADD, node, mul());
        else if (consume('-')) node = new_node(ND_SUB, node, mul());
        else return node;
    }
}

Node *mul() {
    Node *node = primary();
    
    while (1) {
        if (consume('*')) node = new_node(ND_MUL, node, primary());
        else if (consume('/')) node = new_node(ND_DIV, node, primary());
        else return node;
    }
}

Node *primary() {
    if (consume('(')) {
        Node *node = expr();
        expected(')');
        return node;
    } else {
        return new_node_num(expected_num());
    }
}

void gen(Node *node) {
    if (node->type == ND_NUM) {
        printf("    push %d\n", node->val);
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
    }

    printf("    push rax\n");
}

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