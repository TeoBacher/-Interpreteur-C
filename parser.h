#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

//Token currentToken;

// Table des symboles
typedef struct {
    char identifier[256];  // Name of the variable
    int value;             // Value associated with the variable
} SymbolTableEntry;

extern SymbolTableEntry symbolTable[100]; // Max size of the symbol table
extern int symbolCount;                   // Variable counter

void nextToken();

void match(TokenType expected);

int factor();

int term();

int expression();

int exprTail(int lvalue);

int termTail(int lvalue);

int power(int value);

void parsePrintStatement();

int lookupVariable(const char *name);   // Recherche la valeur d'une variable
void assignVariable(const char *name, int value);  // Affecte une valeur à une variable

#endif