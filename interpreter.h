#ifndef interpreter_h
#define interpreter_h

#include "ASTnode.h"
#include "typeclass.h"
#include "value.h"
#include <variant>
#include <unordered_map>

class Interpreter : public AstnodeOperator
{
    struct StackValue
    {
        std::string name;
        std::unique_ptr<Value> value;
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

    Value* copyValue(std::unique_ptr<Value>& value)
    {
        Value* ptr = value.get();
        switch(value->type)
        {
            case ValueType::INTEGER:
                return new IntegerValue{castValue<IntegerValue>(ptr)->number};
            case ValueType::REAL:
                return new RealValue{castValue<RealValue>(ptr)->number};
            case ValueType::CHAR:
                return new CharacterValue{castValue<CharacterValue>(ptr)->character};
            case ValueType::BOOL:
                return new BoolValue{castValue<BoolValue>(ptr)->value};
        }

        return nullptr;
    }
    
    Value* execute(Name* node) override
    {
        return copyValue(stack[getIndex(node->name)].value);
    }
    
    Value* execute(Program* node) override
    {
        return dynamic_cast<Value*>(node->node->execute(this));      
    }
    
    Value* execute(Integer* node) override
    {
        return new IntegerValue{node->number};      
    }

    Value* execute(Double* node) override
    {
        return new RealValue{node->number};      
    }

    Value* execute(Character* node) override
    {
        return new CharacterValue{node->c};      
    }

    Value* execute(Boolean* node) override
    {
        return new BoolValue{node->value};      
    }

    template<typename T>
    T* castValue(Value* value)
    {
        return dynamic_cast<T*>(value);
    }
    
    Value* execute(Unary* node) override
    {
        Value* value = executeNode(node->child.get());
        TokenType tokenType = node->token->type;
        
        if(checkType(node, Type::INTEGER) && tokenType == TokenType::MINUS)
        {
            castValue<IntegerValue>(value)->number = -castValue<IntegerValue>(value)->number;
        }

        if(checkType(node, Type::DOUBLE) && tokenType == TokenType::MINUS)
        {
            castValue<RealValue>(value)->number = -castValue<RealValue>(value)->number;
        }
        
        if(tokenType == TokenType::NOT)
        {
            castValue<BoolValue>(value)->value = !castValue<BoolValue>(value);
        }

        return value;
    }

    Value* execute(Term* node) override
    {
        Value* left = executeNode(node->left.get());
        Value* right = executeNode(node->right.get());

        ASTnode* leftNode = node->left.get();
        ASTnode* rightNode = node->right.get();
        
        TokenType tokenType = node->token->type;

        Value* resValue;
        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::DOUBLE))
        {
            double a = castValue<RealValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            double res = arithmeticOp<double, double, double>(a, b, tokenType);
            resValue = new RealValue{res};
        }
        else if (checkType(leftNode, Type::DOUBLE, rightNode, Type::INTEGER))
        {
            double a = castValue<RealValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            double res = arithmeticOp<double, int, double>(a, b, tokenType);
            resValue = new RealValue{res};
        }
        else if (checkType(leftNode, Type::INTEGER, rightNode, Type::DOUBLE))
        {
            int a = castValue<IntegerValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            double res = arithmeticOp<int, double, double>(a, b, tokenType);
            resValue = new RealValue{res};
        }
        else if (checkType(leftNode, Type::INTEGER, rightNode, Type::INTEGER))
        {
            int a = castValue<IntegerValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            int res = arithmeticOp<int, int, int>(a, b, tokenType);
            resValue = new IntegerValue{res};
        }

        delete left;
        delete right;
        return resValue;
    }

    Value* execute(Factor* node) override
    {
        Value* left = executeNode(node->left.get());
        Value* right = executeNode(node->right.get());

        ASTnode* leftNode = node->left.get();
        ASTnode* rightNode = node->right.get();

        TokenType tokenType = node->token->type;

        Value* resValue;
        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::DOUBLE))
        {
            double a = castValue<RealValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            double res = arithmeticOp<double, double, double>(a, b, tokenType);
            resValue = new RealValue{res};
        }
        else if (checkType(leftNode, Type::DOUBLE, rightNode, Type::INTEGER))
        {
            double a = castValue<RealValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            double res = arithmeticOp<double, int, double>(a, b, tokenType);
            resValue = new RealValue{res};
        }
        else if (checkType(leftNode, Type::INTEGER, rightNode, Type::DOUBLE))
        {
            int a = castValue<IntegerValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            double res = arithmeticOp<int, double, double>(a, b, tokenType);
            resValue = new RealValue{res};
        }
        else if (checkType(leftNode, Type::INTEGER, rightNode, Type::INTEGER))
        {
            int a = castValue<IntegerValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            
            int res = tokenType == TokenType::PERCENT ?
                      a % b : arithmeticOp<int, int, int>(a, b, tokenType);

            resValue = new IntegerValue{res};
        }

        delete left;
        delete right;
        return resValue;
    }
    
    Value* execute(Relational* node) override
    {
        Value* left = executeNode(node->left.get());
        Value* right = executeNode(node->right.get());

        ASTnode* leftNode = node->left.get();
        ASTnode* rightNode = node->right.get();

        TokenType tokenType = node->token->type;
        
        Value* resValue;
        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::DOUBLE))
        {
            double a = castValue<RealValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            bool res = RelationalOp<double, double>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftNode, Type::DOUBLE, rightNode, Type::INTEGER))
        {
            double a = castValue<RealValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            bool res = RelationalOp<double, int>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftNode, Type::INTEGER, rightNode, Type::DOUBLE))
        {
            int a = castValue<IntegerValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            bool res = RelationalOp<int, double>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftNode, Type::INTEGER, rightNode, Type::INTEGER))
        {
            int a = castValue<IntegerValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            bool res = RelationalOp<int, int>(a, b, tokenType);
            resValue = new BoolValue{res};
        }

        delete left;
        delete right;
        return resValue;
    }
    
    Value* execute(Equivalent* node) override
    {
        Value* left = executeNode(node->left.get());
        Value* right = executeNode(node->right.get());

        ASTnode* leftNode = node->left.get();
        ASTnode* rightNode = node->right.get();

        TokenType tokenType = node->token->type;
        
        Value* resValue;
        if (checkType(leftNode, Type::DOUBLE, rightNode, Type::DOUBLE))
        {
            double a = castValue<RealValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            bool res = EquivalentOp<double, double>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftNode, Type::DOUBLE, rightNode, Type::INTEGER))
        {
            double a = castValue<RealValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            bool res = EquivalentOp<double, int>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftNode, Type::INTEGER, rightNode, Type::DOUBLE))
        {
            int a = castValue<IntegerValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            bool res = EquivalentOp<int, double>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftNode, Type::INTEGER, rightNode, Type::INTEGER))
        {
            int a = castValue<IntegerValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            bool res = EquivalentOp<int, int>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftNode, Type::CHARACTER, rightNode, Type::CHARACTER))
        {
            char a = castValue<CharacterValue>(left)->character;
            char b = castValue<CharacterValue>(right)->character;
            bool res = EquivalentOp<char, char>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftNode, Type::BOOLEAN, rightNode, Type::BOOLEAN))
        {
            bool a = castValue<IntegerValue>(left)->number;
            bool b = castValue<IntegerValue>(right)->number;
            bool res = EquivalentOp<bool, bool>(a, b, tokenType);
            resValue = new BoolValue{res};
        }

        delete left;
        delete right;
        return resValue;
    }

    Value* And(Logical* node)
    {
        Value* temp = executeNode(node->left.get());

        if (!castValue<BoolValue>(temp)->value)
        {
            return temp;
        }
        else
        {
            delete temp;
            return executeNode(node->right.get());
        } 
    }

    Value* Or(Logical* node)
    {
        Value* temp = executeNode(node->left.get());

        if (castValue<BoolValue>(temp)->value)
        {
            return temp;
        }
        else
        {
            delete temp;
            return executeNode(node->right.get());
        } 
    }
    
    Value* execute(Logical* node) override
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

    Value* execute(VarDecl* node) override
    {
        std::unique_ptr<Value> value{executeNode(node->expr.get())};
        stack.push_back(StackValue{node->name, std::move(value), currentScope});
        return nullptr;   
    }

    Value* execute(SetVar* node) override
    {
        std::unique_ptr<Value> value{executeNode(node->expr.get())};
        stack[getIndex(node->name)].value = std::move(value);
        return nullptr;   
    }

    Value* execute(PrintStmt* node) override
    {
        Value* value = executeNode(node->expr.get());
        
        switch(value->type)
        {
            case ValueType::INTEGER:
                std::cout<<castValue<IntegerValue>(value)->number<<"\n";
                break;
            case ValueType::REAL:
                std::cout<<castValue<RealValue>(value)->number<<"\n";
                break;
            case ValueType::CHAR:
                std::cout<<castValue<CharacterValue>(value)->character<<"\n";
                break;
            case ValueType::BOOL:
                std::cout<<castValue<BoolValue>(value)->value<<"\n";
                break;
        }

        delete value;
        return nullptr;   
    }
    
    Value* execute(StatementList* node) override
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
        Value* value = executeNode(node->condition.get());

        if (castValue<BoolValue>(value)->value)
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
        Value* value = executeNode(node->condition.get());

        if (castValue<BoolValue>(value)->value)
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
            std::unique_ptr<Value> value{executeNode(node->condition.get())};
            if (!castValue<BoolValue>(value.get())->value) break;

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
            std::unique_ptr<Value> value{executeNode(node->condition.get())};
            if (!castValue<BoolValue>(value.get())->value) break;
            
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
    
    Value* executeNode(ASTnode* const node)
    {
        if (node == nullptr) return nullptr;
        return dynamic_cast<Value*>(node->execute(this));
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
    R arithmeticOp(A a, B b, TokenType type)
    {
        if (type == TokenType::STAR)
        {
            return a * b;
        }
        if (type == TokenType::SLASH)
        {
            return a / b;
        }
        if (type == TokenType::PLUS)
        {
            return a + b;
        }
        if (type == TokenType::MINUS)
        {
            return a - b;
        }

        return R{};
    }
    
    template<typename A, typename B>
    bool RelationalOp(A a, A b, TokenType type)
    {
        if (type == TokenType::LESS)
        {
            return a < b;
        }
        if (type == TokenType::GREAT)
        {
            return a > b;
        }
        if (type == TokenType::LESS_EQUAL)
        {
            return a <= b;
        }
        if (type == TokenType::GREAT_EQUAL)
        {
            return a >= b;
        }

        return false;
    }
    
    template<typename A, typename B>
    bool EquivalentOp(A a, B b, TokenType type)
    {
        if (type == TokenType::EQUAL_EQUAL)
        {
            return a == b;
        }
        if (type == TokenType::NOT_EQUAL)
        {
            return a != b;
        }

        return false;
    }
    
};

#endif
