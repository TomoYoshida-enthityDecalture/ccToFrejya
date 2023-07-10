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
    int len; 
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

//ll tree Node type
typedef enum {
    ND_NUM,
    ND_EQL,
    ND_NEQ,
    ND_LEQ,
    ND_GEQ,
    ND_LE,
    ND_GE,
    ND_ADD, 
    ND_SUB,
    ND_MUL,
    ND_DIV,
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
Node *equality();
Node *relation();
Node *add();
Node *mul();
Node *unary();
Node *primary();

Node *expr() {
    return equality();
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