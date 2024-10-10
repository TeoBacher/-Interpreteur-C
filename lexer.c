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
#include <stdlib.h>
#include "parser.h"

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

// Function for getting the next token
Token getNextToken()
{

    int plusCounter = 0;
    int minusCounter = 0;

    // Return Eof if the end of the input is reached
    while (peek() != '\0')
    {
        char current_char = peek();

        // Skip whitespaces
        if (isspace(current_char))
        {
            advance();
            continue;
        }

        // Check if the current character is a digit
        if (isdigit(current_char))
        {
            char number[256];
            int i = 0;
            while (isdigit(peek()))
            {
                number[i++] = peek();
                advance();
            }
            number[i] = '\0';

            return createToken(Number, number);
        }

        // Check if the current character is an identifier
        if (isalpha(current_char))
        {
            char id[256];
            int i = 0;
            while (isalpha(peek()))
            {
                id[i++] = peek();
                advance();
            }
            id[i] = '\0';

            // Check if the identifier is 'print'
            if (strcmp(id, "print") == 0) {
                return createToken(Print, "print");
            }

            return createToken(Identifier, id);
        }

        // check for arithmetic operators
        switch (peek())
        {
        case '=':
            advance();
            return createToken(Assign, "=");

        case '+':
            advance();
            plusCounter = 1;
            if (peek() == '+')
            {
                advance();
                plusCounter++;
            }

            if (plusCounter == 2)
            {
                return createToken(Inc, "++");
            }
            return createToken(Add, "+");

        case '-':
            advance();
            minusCounter = 1;
            if (peek() == '-')
            {
                advance();
                minusCounter++;
            }

            if (minusCounter == 2)
            {
                return createToken(Dec, "--");
            }
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

        case '<':
            advance();
            if (peek() == '=')
            {
                advance();
                return createToken(Le, "<=");
            }
            return createToken(Lt, "<");

        case '>':
            advance();
            if (peek() == '=')
            {
                advance();
                return createToken(Ge, ">=");
            }
            return createToken(Gt, ">");

        case '!':
            advance();
            if (peek() == '=')
            {
                advance();
                return createToken(Ne, "!=");
            }
            return createToken(Error, "Syntax error or unknown character");

        default:
            return createToken(Error, "Syntax error or unknown character");
        }
    }
    return createToken(Eof, "");
}

void interpret(const char* inputExpression) {
    input = inputExpression;
    position = 0;

    if (debugMode) {
        printf("\nExpression: %s\n", input);
        printf("Début de l'analyse lexicale\n");
    }

    Token token;
    do {
        token = getNextToken();
        if (debugMode) {
            printf("Token: Type = %d, Valeur = %s\n", token.type, token.value);
        }

        if (token.value != NULL) {
            free(token.value);
        }
    } while (token.type != Eof);

    if (debugMode) {
        printf("Fin de l'analyse lexicale\n\n");
    }

    position = 0;
    nextToken();

    int result = expression();

    // if (strstr(input, "print") == NULL) {
    //     printf("Résultat : %d\n\n", result);
    // }
}

// Function to read and interpret a line file by line
void interpretFile(const char* fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char line[256]; 
    while (fgets(line, sizeof(line), file)) {
        // Remove the return to the line at the end of each line
        line[strcspn(line, "\n")] = 0;

        interpret(line);
    }

    fclose(file);
}

// Function for interactive repl mode
void interactiveMode() {
    char inputLine[256];

    printf("Mode interactif. Tapez 'exit' pour quitter.\n");

    while (1) {
        printf("> ");
        if (fgets(inputLine, sizeof(inputLine), stdin) == NULL) {
            break;
        }

        inputLine[strcspn(inputLine, "\n")] = 0;

        if (strcmp(inputLine, "exit") == 0) {
            break;
        }

        interpret(inputLine);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "1") == 0) {
        debugMode = 1;
    }
    
    int mode = 0;
    
    do{
        printf("Choisissez le mode d'exécution:\n");
        printf("1. Mode fichier\n");
        printf("2. Mode interactif\n");
        printf("Entrez votre choix (1 ou 2): ");
        scanf("%d", &mode);
        getchar();
    }while(mode != 1 && mode != 2);
    

    if (mode == 1) {
        interpretFile("code.txt");
    } else if (mode == 2) {
        interactiveMode();
    }

    // interpretFile("code.txt");

    // interactiveMode();

    // interpret("2 ^ 3");
    // interpret("2 ^ 3 ^ 2");
    // interpret("10 ^ 2");
    // interpret("5 < 10");
    // interpret("10 <= 10");
    // interpret("15 > 10");
    // interpret("20 >= 20");
    // interpret("5 != 10");
    // interpret("2 + 3 < 5");
    // interpret("10 - 5 <= 5");
    // interpret("2 * 3 > 5");
    // interpret("10 / 2 >= 5");
    // interpret("10 % 3 != 1");
    // interpret("a = 1 + 1");
    // interpret("b = a + 1");
    // interpret("print(b)");
    // interpret("x = 10 * 2");
    // interpret("y = 100 / 20");
    // interpret("result = x + y");
    // interpret("print(result - 2)");
    // interpret("b = (1 + 2) * (3 - 1)");
    // interpret("print(b)");

    // Array of test expressions
    // const char* testExpressions[] = {
    //     "2 ^ 3",
    //     "2 ^ 3 ^ 2",
    //     "10 ^ 2",
    //     "5 < 10",
    //     "10 <= 10",
    //     "15 > 10",
    //     "20 >= 20",
    //     "5 != 10",
    //     "2 + 3 < 5",
    //     "10 - 5 <= 5",
    //     "2 * 3 > 5",
    //     "10 / 2 >= 5",
    //     "10 % 3 != 1",
    //     "a = 1 + 1",
    //     "b = a + 1",
    //     "print(b)",
    //     "x = 10 * 2",
    //     "y = 100 / 20",
    //     "result = x + y",
    //     "print(result - 2)",
    //     "b = (1 + 2) * (3 - 1)",
    //     "print(b)"
    // };

    // Number of test expressions
    // int numTests = sizeof(testExpressions) / sizeof(testExpressions[0]);

    // for (int i = 0; i < numTests; i++)
    // {
    //     input = testExpressions[i];
    //     position = 0;

    //     printf("Expression: %s\n", input);
    //     printf("Début de l'analyse lexicale\n");

    //     Token token;
    //     do
    //     {
    //         token = getNextToken();
    //         printf("Token: Type = %d, Valeur = %s\n", token.type, token.value);

    //         if (token.value != NULL)
    //         {
    //             free(token.value);
    //         }
    //     } while (token.type != Eof);

    //     printf("Fin de l'analyse lexicale\n");

    //     position = 0;
    //     nextToken();

    //     int result = expression();
    //     //printf("Résultat : %d\n\n", result);
    // }

    return 0;
}