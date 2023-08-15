#ifndef INCLUDE_GUARD_CCTOFREYJA_H
#define INCLUDE_GUARD_CCTOFREYJA_H

#include "token.h"
#include "parse.h"
#include "codegen.h"

//current token
extern Token *current;

//input program
extern char *user_input;

extern Node *code[100];

#endif