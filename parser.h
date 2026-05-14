#ifndef parser_h
#define parser_h

// precedence (left to right)
// ()
// - (unary)
// *, /
// +, -

// term: factor (( "+" | "-") factor)* 
// factor: unary (( "*" | "/") unary)* 
// unary: "-" primary
// primary: NUMBER | "(" term ")"

#include "common.h"
#include "token.h"
#include "ASTnode.h"

class Parser
{
    int current = 0;
    std::vector<Token>& tokens;
    Token* peek();
    bool check(TokenType a);
    bool check(TokenType a, TokenType b);
    Token* consume();
    Token* consume(TokenType a, const char* message);
    bool isAtEnd();
    
    public:

    Parser(std::vector<Token>& _tokens);
    ASTnode* parse();
    ASTnode* term();
    ASTnode* factor();
    ASTnode* unary();
    ASTnode* primary();
    ASTnode* number();
    bool hadError = false;
};

#endif
