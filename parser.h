#ifndef parser_h
#define parser_h

#include "common.h"
#include "token.h"
#include "ASTnode.h"

class Parser
{
    int current = 0;
    std::vector<Token>& tokens;
    bool check(TokenType a);
    bool check(TokenType a, TokenType b);
    Token* consume();
    Token* consume(TokenType a, const char* message);
    bool isAtEnd();
    
    public:

    Parser(std::vector<Token>& _tokens);
    ASTnode* parse();
    ASTnode* unary();
    ASTnode* primary();
    bool hadError = false;
};

#endif
