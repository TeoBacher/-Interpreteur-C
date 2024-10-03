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
    Error = 129,   // Print error
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

    int plusCounter = 0;
    int minusCounter = 0;

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

        // check for +, ++, *, /, %, =, -, --
        switch (peek())
        {
        case '=':
            advance();
            return create_token(Assign, "+");

        case '+':
            while (peek() == '+')
            {
                advance();
                plusCounter++;
                if (plusCounter > 2)
                {   
                    return create_token(Error, "Syntax error");
                }
            }
 
            return create_token(Assign, "+");
        
            if (plusCounter == 2)
            {
               return create_token(Dec, "++");
            }

        case '-':
            while (peek() == '-')
            {
                advance();
                minusCounter++;
                if (minusCounter > 2)
                {
                    return create_token(Error, "Syntax error");
                }
            }
            return create_token(Assign, "-");

            if (minusCounter == 2)
            {
                return create_token(Dec, "--");
            }

        case '*':
            advance();
            return create_token(Mul, "*");

        case '/':
            advance();
            return create_token(Div, "/");

        case '%':
            advance();
            return create_token(Mod, "%");

        default:
            return create_token(Error, "Syntax error or unknown character");
        }   
    }
    
}