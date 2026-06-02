#include "common.h"
#include "parser.h"
#include "ASTnode.h"

Parser::Parser(std::vector<Token>& _tokens) : tokens{_tokens} {}

Program* Parser::parse()
{
    try
    {
        ASTnode* node = statementList();
        consume(TokenType::END, "Unexpected symbol.");
        return new Program(node);
    }
    catch(Error error)
    {
        error.print();
        hadError = true;
    }

    return nullptr;
}

ASTnode* Parser::statementList()
{
    StatementList* stmts = new StatementList{};
    
    while(!isAtEnd())
    {
        std::unique_ptr<ASTnode> node{statement()};
        stmts->list.push_back(std::move(node));
    }

    return stmts;   
}

ASTnode* Parser::statement()
{
    if (check(TokenType::INT, TokenType::REAL) || check(TokenType::BOOL, TokenType::CHAR))
    {
        return varDeclaration();
    }

    if (check(TokenType::SET))
    {
        return setVar();
    }

    if (check(TokenType::PRINT))
    {
        return printStmt();
    }

    return logical();
}

ASTnode* Parser::printStmt()
{
    consume();
    ASTnode* node = logical();
    consume(TokenType::SEMI_COLON, "Expected ';' after statement.");
    return new PrintStmt{node};
}

ASTnode* Parser::varDeclaration()
{
    Token* dataType = consume();
    Token* name = consume(TokenType::IDENTIFIER, "Expect name in variable declaration.");

    ASTnode* expr = nullptr;
    if(check(TokenType::EQUAL))
    {
        consume();
        expr = logical();
    }

    consume(TokenType::SEMI_COLON, "Expected ';' after statement.");

    std::string str{name->start, name->length};
    return new VarDecl(dataType, str, expr, name);
}

ASTnode* Parser::setVar()
{
    consume();
    Token* name = consume(TokenType::IDENTIFIER, "Expect name in assigment.");
    consume(TokenType::EQUAL, "Expected '=' after name.");
    ASTnode* expr = logical();
    consume(TokenType::SEMI_COLON, "Expected ';' after statement.");

    std::string str{name->start, name->length};
    return new SetVar(str, expr, name);
}

ASTnode* Parser::logical()
{
    ASTnode* left = equivalent();

    while(check(TokenType::AND, TokenType::OR))
    {
        Token* token = consume();
        ASTnode* right = equivalent();
        left = new Logical(left, right, token);
    }
    
    return left;
}

ASTnode* Parser::equivalent()
{
    ASTnode* left = relational();

    while(check(TokenType::EQUAL_EQUAL, TokenType::NOT_EQUAL))
    {
        Token* token = consume();
        ASTnode* right = relational();
        left = new Equivalent(left, right, token);
    }
    
    return left;
}

ASTnode* Parser::relational()
{
    ASTnode* left = term();

    while(check(TokenType::LESS, TokenType::GREAT)
          || check(TokenType::LESS_EQUAL, TokenType::GREAT_EQUAL))
    {
        Token* token = consume();
        ASTnode* right = term();
        left = new Relational(left, right, token);
    }
    
    return left;
}

ASTnode* Parser::term()
{
    ASTnode* left = factor();

    while(check(TokenType::PLUS, TokenType::MINUS))
    {
        Token* token = consume();
        ASTnode* right = factor();
        left = new Term(left, right, token);
    }
    
    return left;
}

ASTnode* Parser::factor()
{
    ASTnode* left = unary();

    while(check(TokenType::STAR, TokenType::SLASH) || check(TokenType::PERCENT))
    {
        Token* token = consume();
        ASTnode* right = unary();
        left = new Factor(left, right, token);
    }
    
    return left;
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
    
    if (check(TokenType::IDENTIFIER))
    {
        Token* token = consume();
        std::string str{token->start, token->length};
        return new Name{str, token};
    }
    
    throw Error{&tokens[current], "An expression is expected."};
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
    throw Error{&tokens[current], "Parser error: Unable to consume token."};
}

Token* Parser::consume(TokenType type, std::string message)
{
    if (current < tokens.size() && check(type)) return &tokens[current++];
    throw Error{&tokens[current], message};
}

bool Parser::isAtEnd()
{
    return tokens[current].type == TokenType::END;
}
