#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

Token currentToken;

void nextToken();

void match(TokenType expected);

int factor();

int term();

int expression();

int exprTail(int lvalue);

int termTail(int lvalue);

int power(int value);

#endif