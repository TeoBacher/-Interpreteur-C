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

// Symbol table
typedef struct {
    char identifier[256];  // Name of the variable
    VariableType type;      
    union {                 
        int intValue;
        float floatValue;
        char stringValue[256];
        int boolValue;     
    } value;
} SymbolTableEntry;

extern SymbolTableEntry symbolTable[100]; // Max size of the symbol table
extern int symbolCount;                   // Number of entries in the symbol table


typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_UNDEFINED
} VariableType;

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

int lookupVariable(const char *name);   // Research the value of a variable

void  assignVariable(const char *name, VariableType type, void* value);  // Assign a value to a variable

void checkVariableType(VariableType expected, VariableType actual)

#endif