#ifndef INCLUDE_GUARD_TOKEN_H
#define INCLUDE_GUARD_TOKEN_H

//type of Token
typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_KEYWORD,
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

//token
Token *tokenize(char *p);
void error(char *fmt, ...);
bool consume(char *op);
void expected(char *op);
bool expected_ident();
int consume_ident_len();
bool consume_keyword(char *op);
char *consume_ident();
int expected_num();
bool at_eof();

#endif