#ifndef INCLUDE_GUARD_CCTOFREYJA_C
#define INCLUDE_GUARD_CCTOFREYJA_C

//type of Token
typedef enum {
    TK_RESERVED,
    TK_IDENT,
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
    ND_ASSIGN,
    ND_IDENT,
    ND_LVAR,
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

//current token
extern Token *current;

//input program
extern char *user_input;

extern Node *code[100];

//token
Token *tokenize(char *p);
void error(char *fmt, ...);
bool consume(char *op);
void expected(char *op);
bool consume_ident();
char *expected_ident();
int expected_num();
bool at_eof();

//parse
void program();

//codegen
void gen(Node *node);

#endif