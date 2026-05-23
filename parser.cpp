#include "common.h"
#include "parser.h"
#include "ASTnode.h"

Parser::Parser(std::vector<Token>& _tokens) : tokens{_tokens} {}

ASTnode* Parser::parse()
{
    return unary();
}

ASTnode* Parser::unary()
{
    if (check(TokenType::MINUS, TokenType::NOT))
    {
        Token* token = consume();
        ASTnode* child = primary();
        return new Unary(child, token);   
    }

    return primary();
}

ASTnode* Parser::primary()
{   
    if (check(TokenType::INTEGER))
    {
        Token* token = consume();
        std::string numStr{token->start, token->length};
        return new Integer(std::stoi(numStr), token);
    }

    if (check(TokenType::DOUBLE))
    {
        Token* token = consume();
        std::string numStr{token->start, token->length};
        return new Double(std::stod(numStr), token);
    }
    
    if (check(TokenType::CHARACTER))
    {
        Token* token = consume();
        char c = *(token->start + 1);
        return new Character(c, token);
    }

    if (check(TokenType::STRING))
    {
        Token* token = consume();
        std::string str{token->start + 1, token->length - 2};
        return new String(str, token);
    }

    if (check(TokenType::TRUE, TokenType::FALSE))
    {
        Token* token = consume();
        bool value = token->type == TokenType::TRUE ? true : false;
        return new Boolean(value, token);
    }
    
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

