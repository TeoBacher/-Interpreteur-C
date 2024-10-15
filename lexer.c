/**
 * @file lexer.c
 * @brief A simple lexical analyzer for tokenizing arithmetic expressions.
 *
 * This file contains the implementation of a lexical analyzer (lexer) that
 * tokenizes a given arithmetic expression into a series of tokens. The lexer
 * supports basic arithmetic operations, parentheses, and identifiers.
 *
 * The lexer processes the input character by character, skipping whitespace,
 * and generating tokens based on the recognized patterns.
 *
 * The main function demonstrates the usage of the lexer by tokenizing a sample
 * arithmetic expression and printing the generated tokens.
 *
 * @note The input expression is hardcoded in the main function for demonstration
 * purposes.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"
#include "input.h"

const char *input;
int position = 0;
int debugMode = 0;


// Function that creates the token
Token createToken(TokenType type, const char *value)
{
    Token token;
    token.type = type;
    token.value = (char *)malloc(strlen(value) + 1);
    if (token.value != NULL)
    {
        strncpy(token.value, value, strlen(value) + 1);
    }
        printf("Creating Token: Type = %d, Value = %s\n", type, value);  // Debug: vérifier le type et la valeur du token

    return token;
}

// Get the character
char peek()
{
    return input[position];
}

// Function for advancing the input
void advance()
{
    position++;
}

void ignoreSpaces()
{
    while (isspace(peek()))
    {
        advance();
    }
}

Token getNextToken() {
    while (peek() != '\0') {
        char current_char = peek();

        ignoreSpaces();

        current_char = peek();  

        if (strncmp(input + position, "int", 3) == 0 && !isalnum(input[position + 3])) {
            position += 3;
            return createToken(Int, "int");
        }

        if (strncmp(input + position, "float", 5) == 0 && !isalnum(input[position + 5])) {
            position += 5;
            return createToken(Float, "float");
        }

        if (strncmp(input + position, "char", 4) == 0 && !isalnum(input[position + 4])) {
            position += 4;
            return createToken(Char, "char");
        }

        if (strncmp(input + position, "printf", 6) == 0 && !isalnum(input[position + 6])) {
            position += 6;  

            ignoreSpaces(); 
            
            current_char = peek();  

            return createToken(Printf, "printf");
        }

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


        // Operator (+, -, *, etc.)
        switch (current_char) {
            case '=':
                advance();
                return createToken(Assign, "=");
            case '+':
                advance();
                return createToken(Add, "+");
            case '-':
                advance();
                return createToken(Sub, "-");
            case '*':
                advance();
                return createToken(Mul, "*");
            case '/':
                advance();
                return createToken(Div, "/");
            case '%':
                advance();
                return createToken(Mod, "%");
            case '^':
                advance();
                return createToken(Pow, "^");
            case '(':
                advance();
                return createToken(Lparen, "(");
            case ')':
                advance();
                return createToken(Rparen, ")");
            default:
                printf("Syntax error at char: '%c', Position: %d\n", current_char, position);  // Debug
                return createToken(Error, "Syntax error or unknown character");
        }
    }
    return createToken(Eof, "");
}


// int main()
// {
//     // Array of test expressions
//     const char* testExpressions[] = {
//         // "2 ^ 3",
//         // "2 ^ 3 ^ 2",
//         // "10 ^ 2",
//         // "5 < 10",
//         // "10 <= 10",
//         // "15 > 10",
//         // "20 >= 20",
//         // "5 != 10",
//         // "(2 + 3) * 2",
//         // "10 - 5 <= 5",
//         // "2 * 3 > 5",
//         // "10 / 2 >= 5",
//         // "10 % 3 != 1",
//         // "printf((2 + 3) * 4 )",
//         "x = 5 + 3",
//         "printf( x )",
//         "y =  x + x + 5",
//         "printf( y )",
//         "y = y * 2",
//         "printf( y )"
//     };

//     // Number of test expressions
//     int numTests = sizeof(testExpressions) / sizeof(testExpressions[0]);

//     for (int i = 0; i < numTests; i++)
//     {
//         input = testExpressions[i];
//         position = 0;

//         printf("Expression: %s\n", input);

//         nextToken();  

//         if (currentToken.type == Printf) {
//     printStatement(); 
//     } 
//     else {
//         ASTNode* ast = expression(); 
//         int result = evaluateAST(ast);
//         printf("Résultat : %d\n", result);
//     }
//     }

//     return 0;
// }


void interpret(const char* inputExpression) {
    input = inputExpression;
    position = 0;

    nextToken();  
    do {
        if (currentToken.type == Printf) {
            printStatement();  
        } else {
            expression();  
        }
        nextToken();  
    } while (currentToken.type != Eof);
}


int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "1") == 0) {
        debugMode = 1;
    }

    // Appel à la gestion des entrées (fichier ou mode interactif)
    handleInput();

    return 0;
}