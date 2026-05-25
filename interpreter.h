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

    InterpreterValue* executeNode(ASTnode* const node)
    {
        return dynamic_cast<InterpreterValue*>(node->execute(this));
    }

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
        InterpreterValue* value = executeNode(node->child.get());
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
        
        if(tokenType == TokenType::NOT)
        {
            value->value = !std::get<bool>(value->value);
        }

        return value;
    }

    template<typename A, typename B, typename R>
    R arithmeticOp(Value a, Value b, TokenType type)
    {
        if (type == TokenType::STAR)
        {
            return std::get<A>(a) * std::get<B>(b);
        }
        if (type == TokenType::SLASH)
        {
            return std::get<A>(a) / std::get<B>(b);
        }

        return R{};
    }
    
    InterpreterValue* execute(Factor* node) override
    {
        InterpreterValue* left = executeNode(node->left.get());
        InterpreterValue* right = executeNode(node->right.get());

        TokenType tokenType = node->token->type;

        if (std::holds_alternative<double>(left->value)
            && std::holds_alternative<double>(right->value))
        {
            left->value = arithmeticOp<double, double, double>(left->value,
                                                           right->value,
                                                           tokenType); 
        }
        if (std::holds_alternative<double>(left->value)
            && std::holds_alternative<int>(right->value))
        {
            left->value = arithmeticOp<double, int, double>(left->value,
                                                        right->value,
                                                        tokenType); 
        }
        if (std::holds_alternative<int>(left->value)
            && std::holds_alternative<double>(right->value))
        {
            left->value = arithmeticOp<int, double, double>(left->value,
                                                        right->value,
                                                        tokenType); 
        }
        if (std::holds_alternative<int>(left->value)
            && std::holds_alternative<int>(right->value))
        {
            if (tokenType == TokenType::PERCENT)
            {
                left->value = std::get<int>(left->value) %
                              std::get<int>(right->value);
            }
            else
            {
                left->value = arithmeticOp<int, int, int>(left->value,
                                                          right->value,
                                                          tokenType);    
            } 
        }

        delete right;
        return left;
    }
};

#endif
