#include "common.h"
#include "parser.h"
#include "ASTnode.h"

Parser::Parser(std::vector<Token>& _tokens) : tokens{_tokens} {}

std::unique_ptr<Program> Parser::parse()
{
    try
    {
        return std::make_unique<Program>(declarationList(), nullptr);    
    }
    catch (Error err)
    {
        hadError = true;
        err.print();
        return nullptr;
    }
}

std::unique_ptr<ASTnode> Parser::declarationList()
{
    std::vector<std::unique_ptr<ASTnode>> list;
    while (true)
    {
        if (isBasicType() || isUserType())
        {
            list.push_back(varDeclaration(true));
        }
        else if (check(TokenType::STRUCT))
        {
            list.push_back(structDeclaration());
        }
        else if (check(TokenType::DEF))
        {
            list.push_back(functionDeclaration());
        }
        else 
        {
            break;
        }
    }
    consume(TokenType::END, "Expected EOF.");    
    return std::make_unique<DeclarationList>(std::move(list), &tokens[current]);
}

std::unique_ptr<ASTnode> Parser::structDeclaration()
{
    consume();   
    Token* token = consume(TokenType::IDENTIFIER, "Expected struct name.");
    std::string name{token->start, token->length};

    consume(TokenType::LEFT_BRACE, "Expected a opening '{'.");
    std::unique_ptr<ASTnode> list = variableList();
    consume(TokenType::RIGHT_BRACE, "Expected a closing '}'.");
    return std::make_unique<StructDeclaration>(name, std::move(list), token);
}

std::unique_ptr<ASTnode> Parser::variableList()
{
    std::vector<std::unique_ptr<ASTnode>> list;
    while (true)
    {
        if (isBasicType() || isUserType())
        {
            list.push_back(varDeclaration(true));
        }
        else
        {
            break;
        }
    }
    return std::make_unique<VarList>(std::move(list), &tokens[current]);
}

std::unique_ptr<ASTnode> Parser::functionDeclaration()
{
    consume();
    Token* token = consume(TokenType::IDENTIFIER, "Expected function name.");
    std::string name{token->start, token->length};

    consume(TokenType::LEFT_PAREN, "Expected '(' before paramters in function declaration.");        
    std::vector<std::unique_ptr<ASTnode>> params;
    if (!check(TokenType::RIGHT_PAREN))
    {
        while(true)
        {
            params.push_back(parameter());

            if (!check(TokenType::COMMA)) break;
            consume();
        }
    }

    consume(TokenType::RIGHT_PAREN, "Expected closing ')' in function declaration.");
    std::unique_ptr<ASTnode> returnType = type();
    std::unique_ptr<ASTnode> funBlock = blockStatement();

    return std::make_unique<FunDeclaration>(name, std::move(params), std::move(returnType),
                                            std::move(funBlock), token);
}

std::unique_ptr<ASTnode> Parser::parameter()
{
    std::unique_ptr<ASTnode> paramType = type();
    Token* token = consume(TokenType::IDENTIFIER, "Expected param name.");
    std::string name{token->start, token->length};
    return std::make_unique<Parameter>(std::move(paramType), name, token);    
}

std::unique_ptr<ASTnode> Parser::blockStatement()
{
    Token* token = consume(TokenType::LEFT_BRACE, "Expected an opening '{'.");
    std::unique_ptr<ASTnode> list = statementList();
    consume(TokenType::RIGHT_BRACE, "Expected a closing '}'.");
    return std::make_unique<Block>(std::move(list), token);
}

std::unique_ptr<ASTnode> Parser::statementList()
{
    std::vector<std::unique_ptr<ASTnode>> list;
    while (true)
    {
        if (isBasicType() || isUserType())
        {
            list.push_back(varDeclaration(true));
        }
        else if (check(TokenType::SET))
        {
            list.push_back(assignmentStatement(true));
        }
        else if (check(TokenType::DO))
        {
            list.push_back(expressionStatement(true));
        }
        else
        {
            break;
        }
    }
    
    return std::make_unique<StatementList>(std::move(list), &tokens[current]);
}

std::unique_ptr<ASTnode> Parser::assignmentStatement(bool semicolon)
{
    Token* token = consume();
    std::unique_ptr<ASTnode> var = variable();
    consume(TokenType::EQUAL, "Expected '=' in assignment statement.");
    std::unique_ptr<ASTnode> expr = logical();

    if (semicolon) consume(TokenType::SEMI_COLON, "Expected semicolon after statement.");
    return std::make_unique<AssignmentStatement>(std::move(var), std::move(expr), token);    
}

std::unique_ptr<ASTnode> Parser::expressionStatement(bool semicolon)
{
    Token* token = consume();
    std::unique_ptr<ASTnode> expr = logical();

    if (semicolon) consume(TokenType::SEMI_COLON, "Expected semicolon after statement.");
    return std::make_unique<ExpressionStatement>(std::move(expr), token);
}

std::unique_ptr<ASTnode> Parser::varDeclaration(bool semicolon)
{
    std::unique_ptr<ASTnode> varType = type();
    Token* token = consume(TokenType::IDENTIFIER, "Expected identifier in variable declaration.");
    std::string name{token->start, token->length};

    std::unique_ptr<ASTnode> expr = nullptr;
    if (check(TokenType::EQUAL))
    {
        consume();
        expr = logical();
    }

    if (semicolon) consume(TokenType::SEMI_COLON, "Expected semicolon after statement.");
    return std::make_unique<VarDecl>(std::move(varType), name, std::move(expr), token);
}

int ToInt(const char* start, size_t length)
{
        const std::string str{start, length};
        return std::stoi(str);
}

double ToDouble(const char* start, size_t length)
{
        const std::string str{start, length};
        return std::stod(str);
}

std::unique_ptr<ASTnode> Parser::array()
{
        Token* token = consume();
        std::vector<std::unique_ptr<ASTnode>> elements;

        while(true)
        {
            elements.push_back(logical());
            if(!check(TokenType::COMMA)) break;
            consume();
        }

        consume(TokenType::RIGHT_BRACE, "Expect '}' at the end of the array.");
        return std::make_unique<Array>(std::move(elements), token);
}

std::unique_ptr<ASTnode> Parser::basicType()
{    
    if (check(TokenType::INT))
    {
        Token* token = consume();
        return std::make_unique<Int>(token);
    }
    else if (check(TokenType::REAL))
    {
        Token* token = consume();
        return std::make_unique<Real>(token);
    }
    else if (check(TokenType::CHAR))
    {
        Token* token = consume();
        return std::make_unique<Char>(token);
    }
    else if (check(TokenType::BOOL))
    {
        Token* token = consume();
        return std::make_unique<Bool>(token);
    }

    throw Error(&tokens[current], "Expected a type.");
}

bool Parser::isBasicType()
{
    return check(TokenType::INT, TokenType::REAL) || check(TokenType::CHAR, TokenType::BOOL);
}

std::unique_ptr<ASTnode> Parser::functionType()
{
    Token* token = consume();
    std::unique_ptr<ASTnode> returnType = type();
    consume(TokenType::LESS, "Expected '<' in function type specifier..");

    std::vector<std::unique_ptr<ASTnode>> paramTypes;
    if (!check(TokenType::GREAT))
    {
        while(true)
        {
            paramTypes.push_back(type());

            if (!check(TokenType::COMMA)) break;
            consume();
        }
    }

    consume(TokenType::GREAT, "Expected closing '>' in function type specifier.");
    return std::make_unique<FnType>(std::move(returnType), std::move(paramTypes), token);
}

std::unique_ptr<ASTnode> Parser::userType()
{
    if (check(TokenType::IDENTIFIER))
    {
        Token* token = consume();
        std::string name{token->start, token->length};
        return std::make_unique<TypeName>(name, token);
    }
    else if (check(TokenType::FN))
    {
        return functionType();
    }
    
    throw Error(&tokens[current], "Expected a type.");
}

bool Parser::isUserType()
{
    return check(TokenType::IDENTIFIER, TokenType::FN);
}

std::unique_ptr<ASTnode> Parser::type()
{
    std::unique_ptr<ASTnode> base = nullptr;

    if (isBasicType())
    {
        base = basicType();
    }
    else
    {
        base = userType();
    }

    if (base == nullptr) throw Error(&tokens[current], "Expected a type.");
    
    if(check(TokenType::LEFT_BRACKET))
    {
        Token* token = consume();
        consume(TokenType::RIGHT_BRACKET, "Expected closing ']' in array declaration.");
        base =  std::make_unique<IsArray>(std::move(base), token);
    }
    
    if(check(TokenType::AMPERSAND))
    {
        Token* token = consume();
        base = std::make_unique<IsReference>(std::move(base), token);
    }

    return base;
}

std::unique_ptr<ASTnode> Parser::logical()
{
    std::unique_ptr<ASTnode> left = equivalent();
    while(check(TokenType::AND, TokenType::OR))
    {
        Token* token = consume();
        std::unique_ptr<ASTnode> right = equivalent();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), token);
    }
    return left;
}

std::unique_ptr<ASTnode> Parser::equivalent()
{
    std::unique_ptr<ASTnode> left = relational();
    while(check(TokenType::EQUAL_EQUAL, TokenType::NOT_EQUAL))
    {
        Token* token = consume();
        std::unique_ptr<ASTnode> right = relational();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), token);
    }
    return left;
}

std::unique_ptr<ASTnode> Parser::relational()
{
    std::unique_ptr<ASTnode> left = term();
    while(check(TokenType::LESS, TokenType::LESS_EQUAL) ||
          check(TokenType::GREAT, TokenType::GREAT_EQUAL))
    {
        Token* token = consume();
        std::unique_ptr<ASTnode> right = term();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), token);
    }
    return left;
}

std::unique_ptr<ASTnode> Parser::term()
{
    std::unique_ptr<ASTnode> left = factor();
    while(check(TokenType::PLUS, TokenType::MINUS))
    {
        Token* token = consume();
        std::unique_ptr<ASTnode> right = factor();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), token);
    }
    return left;
}

std::unique_ptr<ASTnode> Parser::factor()
{
    std::unique_ptr<ASTnode> left = unary();
    while(check(TokenType::STAR, TokenType::SLASH))
    {
        Token* token = consume();
        std::unique_ptr<ASTnode> right = unary();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), token);
    }
    return left;
}

std::unique_ptr<ASTnode> Parser::unary()
{
    if(check(TokenType::MINUS, TokenType::NOT))
    {
        Token* token = consume();
        std::unique_ptr<ASTnode> operand = value();
        return std::make_unique<UnaryOperation>(std::move(operand), token);
    }

    return value();
}

std::unique_ptr<ASTnode> Parser::arrayAllocation()
{
        Token* token = consume();
        std::unique_ptr<ASTnode> dataType = type();
        std::unique_ptr<ASTnode> size = logical();
        consume(TokenType::RIGHT_BRACKET, "']'.");
        return std::make_unique<ArrayAllocation>(std::move(dataType), std::move(size), token);
}

std::unique_ptr<ASTnode> Parser::structValue()
{
    consume();
    Token* token = consume(TokenType::IDENTIFIER, "Expected struct name after '.'.");
    std::string structName{token->start, token->length};

    consume(TokenType::LEFT_BRACE, "Expected '{' after struct name.");    

    std::vector<std::unique_ptr<ASTnode>> elements;
    if (!check(TokenType::RIGHT_BRACE))
    {
        while(true)
        {
            elements.push_back(logical());

            if (!check(TokenType::COMMA)) break;
            consume();
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expected '}' at the end of struct value.");
    return std::make_unique<StructValue>(structName, std::move(elements), token);    
}

std::unique_ptr<ASTnode> Parser::memberAccess(std::unique_ptr<ASTnode> base)
{
    consume();
    Token* token = consume(TokenType::IDENTIFIER, "Expected identifier after '.' operator.");
    std::string name{token->start, token->length};
    return std::make_unique<StructElementAccess>(std::move(base), name, token);
}

std::unique_ptr<ASTnode> Parser::elementAccess(std::unique_ptr<ASTnode> base)
{
    Token* token = consume();
    std::unique_ptr<ASTnode> index = logical();
    consume(TokenType::RIGHT_BRACKET, "Expect ']' after the index in array element access.");
    return std::make_unique<ArrayElementAccess>(std::move(base), std::move(index), token);
}

std::unique_ptr<ASTnode> Parser::functionCall(std::unique_ptr<ASTnode> base)
{
    Token* token = consume();

    std::vector<std::unique_ptr<ASTnode>> args;
    if (!check(TokenType::RIGHT_PAREN))
    {
        while(true)
        {
            args.push_back(logical());

            if (!check(TokenType::COMMA)) break;
            consume();
        }
    }

    consume(TokenType::RIGHT_PAREN, "Expected ')' at the end of function call.");
    return std::make_unique<Call>(std::move(base), std::move(args), token);    
}

std::unique_ptr<ASTnode> Parser::variable()
{
    Token* token = consume();
    std::string name{token->start, token->length};

    std::unique_ptr<ASTnode> base = std::make_unique<Identifier>(name, token);
    while(true)
    {
        if(check(TokenType::DOT))
        {
            base = memberAccess(std::move(base));
        }
        else if (check(TokenType::LEFT_BRACKET))
        {
            base = elementAccess(std::move(base));    
        }
        else if (check(TokenType::LEFT_PAREN))
        {
            base = functionCall(std::move(base));
        }
        else
        {
            break;
        }
    }

    return base;
}

std::unique_ptr<ASTnode> Parser::value()
{
    if (check(TokenType::INTEGER))
    {
        Token* token = consume();
        int num = ToInt(token->start, token->length);
        return std::make_unique<Integer>(num, token);
    }

    if (check(TokenType::DOUBLE))
    {
        Token* token = consume();
        double num = ToDouble(token->start, token->length);
        return std::make_unique<Double>(num, token);
    }
    
    if (check(TokenType::CHARACTER))
    {
        Token* token = consume();
        char c = *(token->start + 1);
        return std::make_unique<Character>(c, token);
    }

    if (check(TokenType::TRUE, TokenType::FALSE))
    {
        Token* token = consume();
        bool value = token->type == TokenType::TRUE ? true : false;
        return std::make_unique<Boolean>(value, token);
    }

    if (check(TokenType::STRING))
    {
        Token* token = consume();
        const std::string str{token->start + 1, token->length - 2};
        return std::make_unique<String>(str, token);
    }

    if (check(TokenType::LEFT_BRACE))
    {
        return array();
    }
    
    if (check(TokenType::LEFT_BRACKET))
    {
        return arrayAllocation();
    }
    
    if (check(TokenType::DOT))
    {
        return structValue();
    }

    if (check(TokenType::IDENTIFIER))
    {
        return variable();
    }
    
    throw Error{&tokens[current], "A value is expected."};
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
