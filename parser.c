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

SymbolTableEntry symbolTable[100];  
int symbolCount = 0;    



void checkVariableType(VariableType expected, VariableType actual){
    if (expected != actual) {
        printf("Type error: expected %d but got %d\n", expected, actual);
        exit(1);
    }
}


// Search for a variable in the symbol table
int lookupVariable(const char *name) {
    printf("Looking for variable: %s\n", name);

    for (int i = 0; i < symbolCount; i++) {
        printf("Checking variable: %s with value: %d\n", symbolTable[i].identifier, symbolTable[i].value.intValue);
        if (strcmp(symbolTable[i].identifier, name) == 0) {
            // printf("Found variable: %s, Value: %d\n", name, symbolTable[i].value);
            return symbolTable[i].value.intValue;
        }
    }

    printf("Error: Variable %s not defined\n", name);
    exit(1);
}




// Assign a value to a variable in the symbol table
void assignVariable(const char *name, VariableType type, void* value) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].identifier, name) == 0) {
            checkTypeCompatibility(symbolTable[i].type, type);
            switch (type) {
                case TYPE_INT:
                    symbolTable[i].value.intValue = *((int*)value);
                    break;
                case TYPE_FLOAT:
                    symbolTable[i].value.floatValue = *((float*)value);
                    break;
                case TYPE_STRING:
                    strncpy(symbolTable[i].value.stringValue, (char*)value, 255);
                    break;
                case TYPE_BOOL:
                    symbolTable[i].value.boolValue = *((int*)value);
                    break;
                default:
                    printf("Type error: unsupported type.\n");
                    exit(1);
            }
        }
    }

    // If the variable does not exist, we add it with its type and value
    if (symbolCount < 100) {
        strncpy(symbolTable[symbolCount].identifier, name, 255);
        symbolTable[symbolCount].type = type;
        switch (type) {
            case TYPE_INT:
                symbolTable[symbolCount].value.intValue = *((int*)value);
                break;
            case TYPE_FLOAT:
                symbolTable[symbolCount].value.floatValue = *((float*)value);
                break;
            case TYPE_STRING:
                strncpy(symbolTable[symbolCount].value.stringValue, (char*)value, 255);
                break;
            case TYPE_BOOL:
                symbolTable[symbolCount].value.boolValue = *((int*)value);
                break;
            default:
                printf("Type error: unsupported type.\n");
                exit(1);
        }
        symbolCount++;
    } else {
        printf("Error: Symbol table full\n");
        exit(1);
    }
}





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
    // printf("Created operator node: Type = %d, Left value = %d, Right value = %d\n", type, left->value, right->value); 
    return node;
}


// Retrieves the next token
void nextToken()
{
    currentToken = getNextToken();
    // printf("NextToken /// Current token in parser: Type = %d, Value = %s\n", currentToken.type, currentToken.value);  // Debug du token reçu par le parser

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

ASTNode* factor() {
    ASTNode* node;

    // printf("Factor ?????? Type = %d, Value = %s\n", currentToken.type, currentToken.value); 

    if (currentToken.type == Lparen) {
        match(Lparen);  
        node = expression(); 
        match(Rparen);  
    }
    else if (currentToken.type == Number) {
        node = createNumberNode(atoi(currentToken.value));  
        match(Number); 
    }
    else if (currentToken.type == Identifier) {
        // printf("Entering Identifier block for: %s\n", currentToken.value);  
        int value = lookupVariable(currentToken.value);  
        node = createNumberNode(value);  

        match(Identifier);  

        return node;  
    }
    else {
        printf("Unexpected token in factor: Type = %d, Value = %s\n", currentToken.type, currentToken.value);
        exit(1);
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
    // printf("Entering term with token: Type = %d, Value = %s\n", currentToken.type, currentToken.value);
    ASTNode* lvalue = factor();;
    return termTail(lvalue);
}

// Manages the addition and subtraction operators
ASTNode* exprTail(ASTNode* lvalue)
{
    while (currentToken.type == Add || currentToken.type == Sub) {
        TokenType op = currentToken.type;
        printf("Found operator: %d\n", op);
        match(op);
        ASTNode* rvalue = term(); 
        lvalue = createOperatorNode(op, lvalue, rvalue);  
    }
    return lvalue;
}

// This function calculates the value of an expression,
// which is defined as a term possibly followed by an addition or subtraction.
ASTNode* expression() {

    if (currentToken.type == Identifier) {
        char varName[256];
        strcpy(varName, currentToken.value); 
        match(Identifier);  

        if (currentToken.type == Assign) {
            match(Assign);  
            
            ASTNode* exprNode = term();  
            exprNode = exprTail(exprNode);  
            
            int value = evaluateAST(exprNode); 
            assignVariable(varName, TYPE_INT, &value);
            
            return createNumberNode(value);  
        } else {
            int value = lookupVariable(varName);  
            return createNumberNode(value);  
        }
    }

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

    free(value);  
}






int evaluateAST(ASTNode* node) {
    if (node->type == Number) {
        return node->value;  
    }

    int leftValue = evaluateAST(node->left);
    int rightValue = evaluateAST(node->right);

    // printf("Evaluating: Left = %d, Right = %d, Operator = %d\n", leftValue, rightValue, node->type);


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
