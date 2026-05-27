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

    InterpreterValue* execute(Program* node) override
    {
        return dynamic_cast<InterpreterValue*>(node->node->execute(this));      
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

    InterpreterValue* execute(Term* node) override
    {
        InterpreterValue* left = executeNode(node->left.get());
        InterpreterValue* right = executeNode(node->right.get());

        Value leftValue = left->value;
        Value rightValue = right->value;

        TokenType tokenType = node->token->type;

        if (checkType<double, double>(leftValue, rightValue))
        {
            leftValue = arithmeticOp<double, double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<double, int>(leftValue, rightValue))
        {
            leftValue = arithmeticOp<double, int, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<int, double>(leftValue, rightValue))
        {
            leftValue = arithmeticOp<int, double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<int, int>(leftValue, rightValue))
        {
            leftValue = arithmeticOp<int, int, int>(leftValue, rightValue, tokenType);    
        }

        left->value = leftValue;
        delete right;
        return left;
    }

    InterpreterValue* execute(Factor* node) override
    {
        InterpreterValue* left = executeNode(node->left.get());
        InterpreterValue* right = executeNode(node->right.get());

        Value leftValue = left->value;
        Value rightValue = right->value;

        TokenType tokenType = node->token->type;

        if (checkType<double, double>(leftValue, rightValue))
        {
            leftValue = arithmeticOp<double, double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<double, int>(leftValue, rightValue))
        {
            leftValue = arithmeticOp<double, int, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<int, double>(leftValue, rightValue))
        {
            leftValue = arithmeticOp<int, double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<int, int>(leftValue, rightValue))
        {
            leftValue = arithmeticOp<int, int, int>(leftValue, rightValue, tokenType);    
        }

        left->value = leftValue;
        delete right;
        return left;
    }
    
    InterpreterValue* execute(Relational* node) override
    {
        InterpreterValue* left = executeNode(node->left.get());
        InterpreterValue* right = executeNode(node->right.get());

        Value leftValue = left->value;
        Value rightValue = right->value;

        TokenType tokenType = node->token->type;
        
        if (checkType<double, double>(leftValue, rightValue))
        {
            leftValue = RelationalOp<double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<double, int>(leftValue, rightValue))
        {
            leftValue = RelationalOp<double, int>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<int, double>(leftValue, rightValue))
        {
            leftValue = RelationalOp<int, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<int, int>(leftValue, rightValue))
        {
            leftValue = RelationalOp<int, int>(leftValue, rightValue, tokenType);    
        }

        left->value = leftValue;
        delete right;
        return left;
    }
    
    InterpreterValue* execute(Equivalent* node) override
    {
        InterpreterValue* left = executeNode(node->left.get());
        InterpreterValue* right = executeNode(node->right.get());

        Value leftValue = left->value;
        Value rightValue = right->value;

        TokenType tokenType = node->token->type;
        
        if (checkType<double, double>(leftValue, rightValue))
        {
            leftValue = EquivalentOp<double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<double, int>(leftValue, rightValue))
        {
            leftValue = EquivalentOp<double, int>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<int, double>(leftValue, rightValue))
        {
            leftValue = EquivalentOp<int, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType<int, int>(leftValue, rightValue))
        {
            leftValue = EquivalentOp<int, int>(leftValue, rightValue, tokenType);    
        }

        if (checkType<char, char>(leftValue, rightValue))
        {
            leftValue = EquivalentOp<char, char>(leftValue, rightValue, tokenType);    
        }

        if (checkType<bool, bool>(leftValue, rightValue))
        {
            leftValue = EquivalentOp<bool, bool>(leftValue, rightValue, tokenType);    
        }

        left->value = leftValue;
        delete right;
        return left;
    }

    InterpreterValue* execute(Logical* node) override
    {
        TokenType tokenType = node->token->type;
        InterpreterValue* temp = executeNode(node->left.get());
        
        if (tokenType == TokenType::AND)
        {
            if (!std::get<bool>(temp->value))
            {
                return temp;
            }
            else
            {
                delete temp;
                return executeNode(node->right.get());
            } 
        } 
        
        if (tokenType == TokenType::OR)
        {
            if (std::get<bool>(temp->value))
            {
                return temp;
            }
            else
            {
                delete temp;
                return executeNode(node->right.get());
            } 
        }

        return nullptr; 
    }
    
    InterpreterValue* executeNode(ASTnode* const node)
    {
        return dynamic_cast<InterpreterValue*>(node->execute(this));
    }

    template<typename L, typename R>
    bool checkType(Value left, Value right)
    {
        return std::holds_alternative<L>(left) &&
               std::holds_alternative<R>(right);
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
        if (type == TokenType::PLUS)
        {
            return std::get<A>(a) + std::get<B>(b);
        }
        if (type == TokenType::MINUS)
        {
            return std::get<A>(a) - std::get<B>(b);
        }
        if (type == TokenType::PERCENT)
        {
            return std::get<int>(a) % std::get<int>(b);
        }

        return R{};
    }
    
    template<typename A, typename B>
    bool RelationalOp(Value a, Value b, TokenType type)
    {
        if (type == TokenType::LESS)
        {
            return std::get<A>(a) < std::get<B>(b);
        }
        if (type == TokenType::GREAT)
        {
            return std::get<A>(a) > std::get<B>(b);
        }
        if (type == TokenType::LESS_EQUAL)
        {
            return std::get<A>(a) <= std::get<B>(b);
        }
        if (type == TokenType::GREAT_EQUAL)
        {
            return std::get<A>(a) >= std::get<B>(b);
        }

        return false;
    }
    
    template<typename A, typename B>
    bool EquivalentOp(Value a, Value b, TokenType type)
    {
        if (type == TokenType::EQUAL_EQUAL)
        {
            return std::get<A>(a) == std::get<B>(b);
        }
        if (type == TokenType::NOT_EQUAL)
        {
            return std::get<A>(a) != std::get<B>(b);
        }

        return false;
    }
    
};

#endif
