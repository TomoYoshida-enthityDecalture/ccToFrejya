#ifndef INCLUDE_GUARD_PARSE_H
#define INCLUDE_GUARD_PARSE_H

typedef struct LVar LVar;

struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
};

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
    ND_ASSIGN,
    ND_IDENT,
    ND_LVAR,
    ND_BLOCK,
    ND_IF,
    ND_ELSE,
    ND_WHILE,
    ND_FOR,
    ND_FOR1,
    ND_FOR2,
    ND_RETURN
} Nodetype;

typedef struct Node Node;

//ll tree Node
struct Node {
    Nodetype type;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
};

//parse
void init_lvar();
void program();

#endif
