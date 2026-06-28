#ifndef interpreter_h
#define interpreter_h

#include "ASTnode.h"
#include "typeclass.h"
#include <variant>
#include <unordered_map>

typedef std::variant<int, double, bool, char, std::string> Value;

class InterpreterValue : public ASTvalue
{
    public:
    Value value;

    InterpreterValue(Value _value) : ASTvalue(), value(_value)
    {
        
    }
};

class Interpreter : public AstnodeOperator
{
    struct StackValue
    {
        std::string name;
        Value value;
        int scope;    
    };
    
    public:
    std::vector<StackValue> stack;
    int currentScope = 0;

    int getIndex(std::string name)
    {
        for (int i = stack.size() - 1; i >= 0; i--)
        {
            if(name == stack[i].name)
            {
                return i;
            }
        }
        return 0;
    }
    
    InterpreterValue* execute(Name* node) override
    {
        Value value = stack[getIndex(node->name)].value;
        return new InterpreterValue{value};   
    }
    
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

    InterpreterValue* execute(Boolean* node) override
    {
        return new InterpreterValue(node->value);      
    }
    
    InterpreterValue* execute(Unary* node) override
    {
        InterpreterValue* value = executeNode(node->child.get());
        TokenType tokenType = node->token->type;
        
        if(checkType(node, Type::INTEGER) && tokenType == TokenType::MINUS)
        {
            value->value = -std::get<int>(value->value);
        }

        if(checkType(node, Type::DOUBLE) && tokenType == TokenType::MINUS)
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

        ASTnode* leftNode = node->left.get();
        ASTnode* rightNode = node->right.get();
        
        Value leftValue = left->value;
        Value rightValue = right->value;

        TokenType tokenType = node->token->type;

        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::DOUBLE))
        {
            leftValue = arithmeticOp<double, double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::INTEGER))
        {
            leftValue = arithmeticOp<double, int, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::INTEGER, rightNode, Type::DOUBLE))
        {
            leftValue = arithmeticOp<int, double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::INTEGER, rightNode, Type::INTEGER))
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

        ASTnode* leftNode = node->left.get();
        ASTnode* rightNode = node->right.get();

        Value leftValue = left->value;
        Value rightValue = right->value;

        TokenType tokenType = node->token->type;

        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::DOUBLE))
        {
            leftValue = arithmeticOp<double, double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::INTEGER))
        {
            leftValue = arithmeticOp<double, int, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::INTEGER, rightNode, Type::DOUBLE))
        {
            leftValue = arithmeticOp<int, double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::INTEGER, rightNode, Type::INTEGER))
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

        ASTnode* leftNode = node->left.get();
        ASTnode* rightNode = node->right.get();

        Value leftValue = left->value;
        Value rightValue = right->value;

        TokenType tokenType = node->token->type;
        
        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::DOUBLE))
        {
            leftValue = RelationalOp<double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::INTEGER))
        {
            leftValue = RelationalOp<double, int>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::INTEGER, rightNode, Type::DOUBLE))
        {
            leftValue = RelationalOp<int, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::INTEGER, rightNode, Type::INTEGER))
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

        ASTnode* leftNode = node->left.get();
        ASTnode* rightNode = node->right.get();
        
        Value leftValue = left->value;
        Value rightValue = right->value;

        TokenType tokenType = node->token->type;
        
        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::DOUBLE))
        {
            leftValue = EquivalentOp<double, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::INTEGER))
        {
            leftValue = EquivalentOp<double, int>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::INTEGER, rightNode, Type::DOUBLE))
        {
            leftValue = EquivalentOp<int, double>(leftValue, rightValue, tokenType); 
        }
        
        if (checkType(leftNode, Type::INTEGER, rightNode, Type::INTEGER))
        {
            leftValue = EquivalentOp<int, int>(leftValue, rightValue, tokenType);    
        }

        if (checkType(leftNode, Type::CHARACTER, rightNode, Type::CHARACTER))
        {
            leftValue = EquivalentOp<char, char>(leftValue, rightValue, tokenType);    
        }

        if (checkType(leftNode, Type::BOOLEAN, rightNode, Type::BOOLEAN))
        {
            leftValue = EquivalentOp<bool, bool>(leftValue, rightValue, tokenType);    
        }

        left->value = leftValue;
        delete right;
        return left;
    }

    InterpreterValue* And(Logical* node)
    {
        InterpreterValue* temp = executeNode(node->left.get());

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

    InterpreterValue* Or(Logical* node)
    {
        InterpreterValue* temp = executeNode(node->left.get());

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
    
    InterpreterValue* execute(Logical* node) override
    {
        TokenType tokenType = node->token->type;
        
        if (tokenType == TokenType::AND)
        {
            return And(node);
        } 
        
        if (tokenType == TokenType::OR)
        {
            return Or(node);
        }

        return nullptr; 
    }

    InterpreterValue* execute(VarDecl* node) override
    {
        TokenType type = node->dataType->type;

        if (node->expr)
        {
            InterpreterValue* value = executeNode(node->expr.get());
            stack.push_back(StackValue{node->name, value->value, currentScope});

            delete value;
        }
        else
        {
            if (type == TokenType::INT)
            {
                stack.push_back(StackValue{node->name, Value{0}, currentScope});
            }
            if (type == TokenType::CHAR)
            {
                stack.push_back(StackValue{node->name, Value{'\0'}, currentScope});
            }
            if (type == TokenType::REAL)
            {
                stack.push_back(StackValue{node->name, Value{0.0}, currentScope});
            }
            if (type == TokenType::BOOL)
            {
                stack.push_back(StackValue{node->name, Value{false}, currentScope});
            }
        }
        return nullptr;   
    }

    InterpreterValue* execute(SetVar* node) override
    {
        InterpreterValue* value = executeNode(node->expr.get());
        stack[getIndex(node->name)].value = value->value;
        delete value;
        return nullptr;   
    }

    InterpreterValue* execute(PrintStmt* node) override
    {
        InterpreterValue* value = executeNode(node->expr.get());

        if (std::holds_alternative<int>(value->value))
        {
            std::cout<<std::get<int>(value->value)<<'\n';
        }
        if (std::holds_alternative<char>(value->value))
        {
            std::cout<<std::get<char>(value->value)<<'\n';
        }
        if (std::holds_alternative<bool>(value->value))
        {
            std::cout<<std::get<bool>(value->value)<<'\n';
        }
        if (std::holds_alternative<double>(value->value))
        {
            std::cout<<std::get<double>(value->value)<<'\n';
        }

        delete value;
        return nullptr;   
    }
    
    InterpreterValue* execute(StatementList* node) override
    {
        for(int i = 0; i < node->list.size(); i++)
        {
            node->list[i]->execute(this);
        }

        return nullptr;
    }
    
    void beginScope()
    {        
        currentScope++;
    }

    void endScope()
    {        
        currentScope--;
        while (!stack.empty())
        {
            if (stack.back().scope <= currentScope) break;
            stack.pop_back();
        }
    }
    
    virtual ASTvalue* execute(BlockStatement* node) override
    {
        beginScope();
        node->stmtList->execute(this);
        endScope();        
        return nullptr;
    }
     
    virtual ASTvalue* execute(IfStatement* node) override
    {
        InterpreterValue* value = executeNode(node->condition.get());

        if (std::get<bool>(value->value))
        {
            node->block->execute(this);
        }
        else if (node->tail)
        {
            node->tail->execute(this);
        }
        
        delete value;
        return nullptr;
    }

    virtual ASTvalue* execute(ElifStatement* node) override
    {
        InterpreterValue* value = executeNode(node->condition.get());

        if (std::get<bool>(value->value))
        {
            node->block->execute(this);
        }
        else if (node->tail)
        {
            node->tail->execute(this);
        }
        
        delete value;
        return nullptr;
    }

    virtual ASTvalue* execute(WhileStatement* node) override
    {
        while(true)
        {
            std::unique_ptr<InterpreterValue> value{executeNode(node->condition.get())};
            if (!std::get<bool>(value->value)) break;

            node->block->execute(this);
        }        
        return nullptr;
    }

    virtual ASTvalue* execute(ForStatement* node) override
    {
        beginScope();

        delete executeNode(node->initializer.get());

        while(true && node->condition == nullptr)
        {
            node->block->execute(this);
            delete executeNode(node->updator.get());
        }
        
        while(true && node->condition)
        {
            std::unique_ptr<InterpreterValue> value{executeNode(node->condition.get())};
            if (!std::get<bool>(value->value)) break;
            
            node->block->execute(this);
            delete executeNode(node->updator.get());
        }
        
        endScope();        
        return nullptr;
    }

    virtual ASTvalue* execute(ElseStatement* node) override
    {
        node->block->execute(this);
        return nullptr;
    }
    
    InterpreterValue* executeNode(ASTnode* const node)
    {
        if (node == nullptr) return nullptr;
        return dynamic_cast<InterpreterValue*>(node->execute(this));
    }

    bool checkType(ASTnode* node, Type expected)
    {
        if (node == nullptr) return false;
        return node->typeClass->baseType == expected;
    }

    bool checkType(ASTnode* left, Type lType, ASTnode* right, Type rType)
    {
        if (left == nullptr || right == nullptr) return false;
        return left->typeClass->baseType == lType && right->typeClass->baseType == rType;
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
