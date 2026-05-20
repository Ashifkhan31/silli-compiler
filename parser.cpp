#include "common.h"
#include "parser.h"
#include "ASTnode.h"
#include <charconv>

Parser::Parser(std::vector<Token>& _tokens) : tokens{_tokens} {}

ASTnode* Parser::parse()
{
    return nullptr;
}

bool Parser::check(TokenType type)
{
    if (current < tokens.size()) return tokens[current].type == type;
    return false;
}

bool Parser::check(TokenType type1, TokenType type2)
{
    return check(type1) || check(type2);
}

Token* Parser::consume()
{
    if (current < tokens.size()) return &tokens[current++];
    std::cout<<"Unable to consume token.\n";
    return nullptr;
}

Token* Parser::consume(TokenType type, const char* message)
{
    if (current < tokens.size() && check(type)) return &tokens[current++];
    hadError = true;
    std::cout<<message<<"\n";
    throw "error";
}

