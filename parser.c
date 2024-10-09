/**
 * @file parser.c
 * @brief This file contains the implementation of a simple arithmetic expression parser.
 *
 * The parser supports basic arithmetic operations including addition, subtraction,
 * multiplication, division, and modulo. It uses recursive descent parsing to evaluate
 * expressions involving numbers and parentheses.
 *
 * The parser relies on the following external functions and types:
 *
 * - Token getNextToken(): Retrieves the next token from the input.
 * - TokenType: An enumeration representing the different types of tokens.
 * - Token: A structure representing a token, with fields for the token type and value.
 *
 * The parser reports syntax errors and exits the program if an unexpected token is encountered.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "parser.h"

Token currentToken;

// Retrieves the next token
void nextToken()
{
    currentToken = getNextToken();
    printf("Current token: Type = %d, Value = %s\n", currentToken.type, currentToken.value);
}

// Matches the current token with the expected token
void match(TokenType expected)
{
    if (currentToken.type == expected)
    {
        nextToken();
    }
    else
    {
        printf("Syntax error: expected %d but got %d\n", expected, currentToken.type);
        exit(1);
    }
}

// Manages the basic elements of arithmetic expressions (numbers and parentheses)
int factor()
{
    int value = 0;
    if (currentToken.type == Lparen)
    {
        match(Lparen);
        value = expression();
        match(Rparen);
    }
    else if (currentToken.type == Number)
    {
        value = atoi(currentToken.value);
        match(Number);
    }
    else
    {
        printf("Syntax error: waiting for a number or a parenthesis\n");
        exit(1);
    }

    // Check for power operator
    if (currentToken.type == Pow)
    {
        match(Pow);
        int exponent = factor();
        value = pow(value, exponent); // Use the math.h library to calculate the power
    }

    return value;
}

// Manages the multiplication, division and modulo operators
int termTail(int lvalue)
{
    while (currentToken.type == Mul || currentToken.type == Div || currentToken.type == Mod)
    {
        TokenType op = currentToken.type;
        match(op);
        int rvalue = factor();
        switch (op)
        {
        case Mul:
            lvalue *= rvalue;
            break;
        case Div:
            lvalue /= rvalue;
            break;
        case Mod:
            lvalue %= rvalue;
            break;
        default:
            printf("Syntax error: unexpected operator\n");
            exit(1);
        }
    }
    return lvalue;
}

// This function calculates the value of a term, which is defined as a factor (a number or a parenthesis)
// possibly followed by multiplication or division.
int term()
{
    int lvalue = factor();
    return termTail(lvalue);
}

// Manages the addition and subtraction operators
int exprTail(int lvalue)
{
    switch (currentToken.type)
    {
    case Add:
        match(Add);
        return exprTail(lvalue + term());
    case Sub:
        match(Sub);
        return exprTail(lvalue - term());
    case Lt:
        match(Lt);
        return exprTail(lvalue < term());
    case Le:
        match(Le);
        return exprTail(lvalue <= term());
    case Gt:
        match(Gt);
        return exprTail(lvalue > term());
    case Ge:
        match(Ge);
        return exprTail(lvalue >= term());
    case Ne:
        match(Ne);
        return exprTail(lvalue != term());
    default:
        return lvalue;
    }
}

int power(int value)
{
    match(Pow);

    int exponent = factor();
    return pow(value, exponent);
}

// This function calculates the value of an expression,
// which is defined as a term possibly followed by an addition or subtraction.
int expression()
{
    int lvalue = term();
    return exprTail(lvalue);
}