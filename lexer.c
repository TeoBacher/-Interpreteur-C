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

const char *input;
int position = 0;

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


int main()
{
    // Array of test expressions
    const char* testExpressions[] = {
        // "2 ^ 3",
        // "2 ^ 3 ^ 2",
        // "10 ^ 2",
        // "5 < 10",
        // "10 <= 10",
        // "15 > 10",
        // "20 >= 20",
        // "5 != 10",
        // "(2 + 3) * 2",
        // "10 - 5 <= 5",
        // "2 * 3 > 5",
        // "10 / 2 >= 5",
        // "10 % 3 != 1",
        "printf( (2 + 3) * 4 )",
    };

    // Number of test expressions
    int numTests = sizeof(testExpressions) / sizeof(testExpressions[0]);

    for (int i = 0; i < numTests; i++)
    {
        input = testExpressions[i];
        position = 0;

        printf("Expression: %s\n", input);

        Token token;
        do
        {
            token = getNextToken();
            if (token.value != NULL)
            {
                free(token.value);
            }
        } while (token.type != Eof);

        position = 0;
        nextToken();

        if (currentToken.type == Printf) {
            printStatement();
        } else {
            ASTNode* ast = expression();
            int result = evaluateAST(ast);
            printf("Résultat : %d\n", result);
        }
    }

    return 0;
}