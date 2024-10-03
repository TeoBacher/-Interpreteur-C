#include <stdio.h>
#include <ctype.h>  
#include <string.h> 

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
}TokenType;


typedef struct {
    TokenType type;
    char value[256];  // Token value
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


// Function for advancing the input
void advance() {
    position++;
}


Token getNextToken(){
    while (peek() != '\0')
    {
        char current_char = peek();

        // Skip whitespaces
        if (isspace(current_char)) {
            advance();
            continue;
        }

        // Check if the current character is a digit
        if (isdigit(current_char)) {
            char number[256];
            int i = 0;
            while (isdigit(peek())) {
                number[i++] = peek();
                advance();
            }
            number[i] = '\0';
            return create_token(Number, number);
        }

        // Check if the current character is an identifier
        if (isalpha(current_char)) {
            char id[256];
            int i = 0;
            while (isalpha(peek())) {
                id[i++] = peek();
                advance();
            }
            id[i] = '\0';
            return create_token(Identifier, id);
        }

        // Check for operator

        if (peek() == '=') {
            advance();
            return create_token(Assign, "=");
        }

        if (peek() == '+') {
            advance();
            return create_token(Add, "+");
        }

        if (peek() == '-') {
            advance();
            return create_token(Sub, "-");
        }

        if (peek() == '*') {
            advance();
            return create_token(Mul, "*");
        }

        if (peek() == '/') {
            advance();
            return create_token(Div, "/");
        }

        if (peek() == '%') {
            advance();
            return create_token(Mod, "%");
        }        
    }
    
}