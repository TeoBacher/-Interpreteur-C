#include <stdio.h>
#include <ctype.h>  
#include <string.h> 

typedef enum {
    Assign = 128,  // '=' assignment operator
    Add,           // '+' addition
    Sub,           // '-' subtraction
    Mul,           // '*' multiplication
    Div,           // '/' division
    Mod,           // '%' modulo
    Inc,           // '++' incrementation
    Dec,           // '--' décrementation
}TokenType;


typedef struct {
    TokenType type;
    char value[256];  // La valeur du token
} Token;

char* input;
int position = 0;

// Function that creates the token
Token create_token(TokenType type, const char* value) {
    Token token;
    token.type = type;
    strncpy(token.value, value, 255);
    return token;
}

// Get the character
char peek() {
    return input[position];
}