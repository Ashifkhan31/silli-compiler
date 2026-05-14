#include "common.h"
#include "parser.h"
#include "ASTnode.h"
#include <charconv>

Parser::Parser(std::vector<Token>& _tokens) : tokens{_tokens} {}

ASTnode* Parser::parse()
{
    try
    {
        ASTnode* node = term();

        if(check(TokenType::END))
        {
            consume();
            return node;
        }

        hadError = true;
        std::cout<<"Expect expression\n";
        throw "error";
    }
    catch (...)
    {
        return nullptr;
    }
}

ASTnode* Parser::term()
{
    ASTnode* left = factor();

    while(check(TokenType::PLUS, TokenType::MINUS))
    {
        Token* operatr = consume();
        ASTnode* right = factor();
        left = new TermNode(left, right, operatr);
    }
    
    return left;
}

ASTnode* Parser::factor()
{
    ASTnode* left = unary();

    while(check(TokenType::STAR, TokenType::SLASH))
    {
        Token* operatr = consume();
        ASTnode* right = unary();
        left = new FactorNode(left, right, operatr);
    }
    
    return left;
}

ASTnode* Parser::unary()
{
    if(check(TokenType::MINUS))
    {
        Token* token = consume();
        ASTnode* node = primary();
        return new UnaryNode(node, token);
    }

    return primary();
}

ASTnode* Parser::primary()
{
    if(check(TokenType::LEFT_PAREN))
    {
        consume();
        ASTnode* node = term();
        consume(TokenType::RIGHT_PAREN, "Expect a ')' after a expression");
        return node;
    }

    if (check(TokenType::NUMBER))
    {
        Token* token = consume();
        double num;
        std::from_chars(token->start, token->start + token->length, num);
        return new NumberNode(num, token);
    }

    hadError = true;
    std::cout<<"Expect expression\n";
    throw "error";
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

