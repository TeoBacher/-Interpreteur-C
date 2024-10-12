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


ASTNode* createNumberNode(int value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = Number;
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

ASTNode* createOperatorNode(TokenType type, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = 0;  
    node->left = left;
    node->right = right;
    return node;
}


// Retrieves the next token
void nextToken()
{
    currentToken = getNextToken();
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
ASTNode* factor()
{
    ASTNode* node;
    printf("Current token in factor: Type = %d, Value = %s\n", currentToken.type, currentToken.value);  // Debug

    if (currentToken.type == Lparen)
    {
        match(Lparen);
        node = expression();
        match(Rparen);
    }
    else if (currentToken.type == Number)
    {
        node = createNumberNode(atoi(currentToken.value));
        match(Number);
    }
    else
    {
        printf("Syntax error: waiting for a number or a parenthesis\n");
        exit(1);
    }

    if (currentToken.type == Pow) {
        match(Pow);
        node = createOperatorNode(Pow, node, factor());  
    }
    return node;
}

// Manages the multiplication, division and modulo operators
ASTNode* termTail(ASTNode* lvalue)
{
    while (currentToken.type == Mul || currentToken.type == Div || currentToken.type == Mod)
    {
        TokenType op = currentToken.type;
        match(op);
        ASTNode* rvalue = factor();
        lvalue = createOperatorNode(op, lvalue, rvalue);  
    }
    return lvalue;
}

// This function calculates the value of a term, which is defined as a factor (a number or a parenthesis)
// possibly followed by multiplication or division.
ASTNode* term()
{
    ASTNode* lvalue = factor();;
    return termTail(lvalue);
}

// Manages the addition and subtraction operators
ASTNode* exprTail(ASTNode* lvalue)
{
    while (currentToken.type == Add || currentToken.type == Sub) {
        TokenType op = currentToken.type;
        match(op);
        ASTNode* rvalue = term(); 
        lvalue = createOperatorNode(op, lvalue, rvalue);  
    }
    return lvalue;
}

// This function calculates the value of an expression,
// which is defined as a term possibly followed by an addition or subtraction.
ASTNode* expression()
{
    ASTNode* lvalue = term(); 
    return exprTail(lvalue);
}

void printStatement() {
    match(Printf);  
    match(Lparen);  

    ASTNode* value = expression();  

    match(Rparen); 

    int result = evaluateAST(value);  

    printf("%d\n", result); 
}



int evaluateAST(ASTNode* node) {
    if (node->type == Number) {
        return node->value;  
    }

    int leftValue = evaluateAST(node->left);
    int rightValue = evaluateAST(node->right);

    switch (node->type) {
        case Add: return leftValue + rightValue;
        case Sub: return leftValue - rightValue;
        case Mul: return leftValue * rightValue;
        case Div: return leftValue / rightValue;
        case Mod: return leftValue % rightValue;
        case Pow: return pow(leftValue, rightValue);
        default: printf("Unknown operator: %d\n", node->type); exit(1);
    }
}
