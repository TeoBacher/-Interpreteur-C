#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define token types
typedef enum
{
    Number = 0,     // 0-9
    Identifier = 1, // a-z, A-Z
    Add,            // '+'
    Sub,            // '-'
    Mul,            // '*'
    Div,            // '/'
    Mod,            // '%'
    Inc,            // '++'
    Dec,            // '--'
    Pow,            // '^'
    Lt,             // '<'
    Le,             // '<='
    Gt,             // '>'
    Ge,             // '>='
    Ne,             // '!='
    Printf,
    Eof,            // End of file
    Lparen,         // '('
    Rparen,         // ')'
    Print,          // For the print command
    Assign = 128,   // '='
    Error = 129,    // Print error
} TokenType;

// Define a token structure
typedef struct
{
    TokenType type;
    char *value;
} Token;

// Function prototypes
Token createToken(TokenType type, const char *value);
char peek();
void advance();
Token getNextToken();
void ignoreSpaces();

#endif