#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

Token currentToken;

typedef struct ASTNode {
    TokenType type;            
    int value;                 
    struct ASTNode* left;      
    struct ASTNode* right;     
} ASTNode;

void match(TokenType expected);

void nextToken();

void printStatement();

ASTNode* factor();

ASTNode* term();

ASTNode* termTail(ASTNode* lvalue);

ASTNode* expression();

ASTNode* exprTail(ASTNode* lvalue);

ASTNode* createNumberNode(int value);

ASTNode* createOperatorNode(TokenType type, ASTNode* left, ASTNode* right);

int evaluateAST(ASTNode* node);


#endif