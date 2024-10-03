#include <stdio.h>
#include <ctype.h>  
#include <string.h> 
#include <lexer.h>



typedef struct {
    TokenType type;
    char value[256];  // Token value
} Token;

char* input;
int position = 0;

// Function that creates the token
Token createToken(TokenType type, const char* value) {
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
            return createToken(Number, number);
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
            return createToken(Identifier, id);
        }

        // check for +, ++, *, /, %, =, -, --
        switch (peek())
        {
        case '=':
            advance();
            return createToken(Assign, "=");

        case '+':
            while (peek() == '+')
            {
                advance();
                plusCounter++;
                if (plusCounter > 2)
                {   
                    return createToken(Error, "Syntax error");
                }
            }
 
            return createToken(Assign, "+");
        
            if (plusCounter == 2)
            {
               return createToken(Dec, "++");
            }

        case '-':
            while (peek() == '-')
            {
                advance();
                minusCounter++;
                if (minusCounter > 2)
                {
                    return createToken(Error, "Syntax error");
                }
            }
            return createToken(Assign, "-");

            if (minusCounter == 2)
            {
                return createToken(Dec, "--");
            }

        case '*':
            advance();
            return createToken(Mul, "*");

        case '/':
            advance();
            return createToken(Div, "/");

        case '%':
            advance();
            return createToken(Mod, "%");

        default:
            return createToken(Error, "Syntax error or unknown character");
        }   
    }
    return createToken(Eof, "");
}

int main() {
    input = "x = 3 + 4 * 2";
    Token token;
    do {
        token = getNextToken();
        printf("Token: Type = %d, Valeur = %s\n", token.type, token.value);
    } while (token.type != Eof);

    return 0;
}