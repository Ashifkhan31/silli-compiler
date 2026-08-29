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
    
    std::unique_ptr<ASTnode> declarationList();
    std::unique_ptr<ASTnode> referDeclaration();

    std::unique_ptr<ASTnode> varDeclaration(bool semicolon = true);

    std::unique_ptr<ASTnode> variableList();
    std::unique_ptr<ASTnode> structDeclaration();
    std::unique_ptr<ASTnode> parameter();
    std::unique_ptr<ASTnode> functionDeclaration();
    std::unique_ptr<ASTnode> statementList();
    std::unique_ptr<ASTnode> assignmentStatement(bool semicolon = true);
    std::unique_ptr<ASTnode> expressionStatement(bool semicolon = true);
    std::unique_ptr<ASTnode> whileStatement();
    std::unique_ptr<ASTnode> forStatement();

    std::unique_ptr<ASTnode> parseCondition();
    std::unique_ptr<ASTnode> ifStatement();
    std::unique_ptr<ASTnode> ifTail();

    std::unique_ptr<ASTnode> breakStatement();
    std::unique_ptr<ASTnode> continueStatement();
    std::unique_ptr<ASTnode> blockStatement();
    std::unique_ptr<ASTnode> logical();
    std::unique_ptr<ASTnode> equivalent();
    std::unique_ptr<ASTnode> relational();
    std::unique_ptr<ASTnode> term();
    std::unique_ptr<ASTnode> factor();
    std::unique_ptr<ASTnode> unary();
    std::unique_ptr<ASTnode> value();
    std::unique_ptr<ASTnode> group();

    std::unique_ptr<ASTnode> array();
    std::unique_ptr<ASTnode> arrayAllocation();

    std::unique_ptr<ASTnode> structValue();
    std::unique_ptr<ASTnode> variable();
    std::unique_ptr<ASTnode> memberAccess(std::unique_ptr<ASTnode> base);
    std::unique_ptr<ASTnode> elementAccess(std::unique_ptr<ASTnode> base);
    std::unique_ptr<ASTnode> functionCall(std::unique_ptr<ASTnode> base);
    std::unique_ptr<ASTnode> cast();
    
    std::unique_ptr<ASTnode> type();
    std::unique_ptr<ASTnode> basicType();
    std::unique_ptr<ASTnode> userType();
    bool isBasicType();
    bool isUserType();
    std::unique_ptr<ASTnode> functionType();
    
    public:
    bool hadError = false;
    Parser(std::vector<Token>& _tokens);
    std::unique_ptr<Program> parse();
};

#endif
