#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define token types
typedef enum {
    Number,        // 0-9
    Identifier,    // a-z, A-Z
    Assign = 128,  // '=' 
    Add,           // '+' 
    Sub,           // '-' 
    Mul,           // '*' 
    Div,           // '/' 
    Mod,           // '%' 
    Inc,           // '++' 
    Dec,           // '--' 
    Eof,           // End of file
    Error = 129,   // Print error
}TokenType;

// Define a token structure
typedef struct {
    TokenType type;
    char *value;
} Token;

// Function prototypes
Token *createToken(TokenType type, const char *value);
char peek();
void advance();
Token *getNextToken();

#endif 