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
    Token* consume(TokenType a, std::string message);
    bool isAtEnd();
    ASTnode* parseInitializer();
    ASTnode* parseCondition();
    ASTnode* parseUpdater();
    ASTnode* ifTail();
    
    public:

    Parser(std::vector<Token>& _tokens);
    Program* parse();
    ASTnode* statementList();
    ASTnode* statement();
    ASTnode* varDeclaration();
    ASTnode* setVar();
    ASTnode* printStmt();
    ASTnode* ifStatement();
    ASTnode* elifStatement();
    ASTnode* elseStatement();
    ASTnode* whileStatement();
    ASTnode* forStatement();
    ASTnode* block();
    ASTnode* logical();
    ASTnode* equivalent();
    ASTnode* relational();
    ASTnode* term();
    ASTnode* factor();
    ASTnode* unary();
    ASTnode* primary();
    bool hadError = false;
};

#endif
