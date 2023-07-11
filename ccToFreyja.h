#ifndef INCLUDE_GUARD_CCTOFREYJA_C
#define INCLUDE_GUARD_CCTOFREYJA_C

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

//current token
extern Token *current;

//input program
extern char *user_input;

//parse
Token *tokenize(char *p);
bool consume(char *op);
void expected(char *op);
int expected_num();

//codegen
Node *expr();
void gen(Node *node);

#endif