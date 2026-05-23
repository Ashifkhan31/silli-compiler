#ifndef interpreter_h
#define interpreter_h

#include "ASTnode.h"
#include <variant>

typedef std::variant<int, double, bool, char, std::string> Value;

class InterpreterValue : public ASTvalue
{
    public:
    Value value;

    InterpreterValue(Value _value) : ASTvalue(), value(_value)
    {
        
    }

    void print()
    {
        if(std::holds_alternative<int>(value))
        {
            std::cout<<std::get<int>(value)<<"\n";
        }         
        if(std::holds_alternative<double>(value))
        {
            std::cout<<std::get<double>(value)<<"\n";
        }         
        if(std::holds_alternative<bool>(value))
        {
            std::cout<<std::get<bool>(value)<<"\n";
        }         
        if(std::holds_alternative<char>(value))
        {
            std::cout<<std::get<char>(value)<<"\n";
        }         
        if(std::holds_alternative<std::string>(value))
        {
            std::cout<<std::get<std::string>(value)<<"\n";
        }         
    }
};

class Interpreter : public AstnodeOperator
{
    public:

    InterpreterValue* execute(ASTnode* node) override
    {
        return dynamic_cast<InterpreterValue*>(AstnodeOperator::execute(node));      
    }
    
    InterpreterValue* execute(Integer* node) override
    {
        return new InterpreterValue(node->number);      
    }

    InterpreterValue* execute(Double* node) override
    {
        return new InterpreterValue(node->number);      
    }

    InterpreterValue* execute(Character* node) override
    {
        return new InterpreterValue(node->c);      
    }

    InterpreterValue* execute(String* node) override
    {
        return new InterpreterValue(node->str);      
    }

    InterpreterValue* execute(Boolean* node) override
    {
        return new InterpreterValue(node->value);      
    }
    
    InterpreterValue* execute(Unary* node) override
    {
        ASTvalue* astValue = node->child->execute(this);
        InterpreterValue* value = dynamic_cast<InterpreterValue*>(astValue);
        TokenType tokenType = node->token->type;
        
        if(std::holds_alternative<int>(value->value) &&
           tokenType == TokenType::MINUS)
        {
            value->value = -std::get<int>(value->value);
        }

        if(std::holds_alternative<double>(value->value) &&
           tokenType == TokenType::MINUS)
        {
            value->value = -std::get<double>(value->value);
        }
        
        if(std::holds_alternative<bool>(value->value) &&
           tokenType == TokenType::NOT)
        {
            value->value = !std::get<bool>(value->value);
        }

        return value;
    }
};

#endif
