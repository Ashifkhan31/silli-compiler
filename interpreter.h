#ifndef interpreter_h
#define interpreter_h

#include "ASTnode.h"
#include "typeclass.h"
#include "value.h"
#include <variant>

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

    template<typename T>
    T* castValue(Value* value)
    {
        return dynamic_cast<T*>(value);
    }

    Value* copyValue(Value* value)
    {
        switch(value->type)
        {
            case ValueType::INTEGER:
                return new IntegerValue{castValue<IntegerValue>(value)->number};
            case ValueType::REAL:
                return new RealValue{castValue<RealValue>(value)->number};
            case ValueType::CHAR:
                return new CharacterValue{castValue<CharacterValue>(value)->character};
            case ValueType::BOOL:
                return new BoolValue{castValue<BoolValue>(value)->value};
            case ValueType::ARRAY:
            {   
                ArrayValue* arr = new ArrayValue{};
                for (std::unique_ptr<Value>& i : castValue<ArrayValue>(value)->elements)
                {
                    std::unique_ptr<Value> elementVal{copyValue(i.get())};
                    arr->elements.push_back(std::move(elementVal));
                }
                return arr;
            }
            case ValueType::REFERENCE:
                return new ReferenceValue{castValue<ReferenceValue>(value)->val};
        }

        return nullptr;
    }

    template<typename T>
    T* unWrapValue(Value* value)
    {
        if (value->type == ValueType::REFERENCE)
        {
            std::unique_ptr<ReferenceValue> ref {dynamic_cast<ReferenceValue*>(value)};
            return dynamic_cast<T*>(copyValue(ref->val));
        }

        return dynamic_cast<T*>(value);
    }    
    
    Value* unWrapValue(Value* value)
    {
        if (value->type == ValueType::REFERENCE)
        {
            std::unique_ptr<ReferenceValue> ref {dynamic_cast<ReferenceValue*>(value)};
            return dynamic_cast<Value*>(copyValue(ref->val));
        }

        return dynamic_cast<Value*>(value);
    }    
    
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

    Value* executeNode(ASTnode* const node)
    {
        if (node == nullptr) return nullptr;
        return dynamic_cast<Value*>(node->execute(this));
    }

    Value* executeNode(std::unique_ptr<ASTnode>& node)
    {
        if (node == nullptr) return nullptr;
        return dynamic_cast<Value*>(node->execute(this));
    }

    bool checkType(std::unique_ptr<TypeClass>& typeClass, Type expected)
    {
        return typeClass->baseType == expected;
    }

    bool checkType(std::unique_ptr<TypeClass>& left, Type lType,
                   std::unique_ptr<TypeClass>& right, Type rType)
    {
        return left->baseType == lType && right->baseType == rType;
    }   

    std::unique_ptr<TypeClass>& unwrapTypeClass(std::unique_ptr<TypeClass>& typeClass)
    {
        if(typeClass->baseType == Type::REFERENCE)
        {
            return dynamic_cast<ReferenceType*>(typeClass.get())->simpleType;    
        }
        
        return typeClass;
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
    
    Value* execute(Program* node) override
    {
        try
        {
            return dynamic_cast<Value*>(node->node->execute(this));
        }
        catch (Error error)
        {
            error.print();
        }
        return nullptr;
    }
    
    Value* execute(Name* node) override
    {
        Value* val = stack[getIndex(node->name)].value.get();
        return new ReferenceValue{val};
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

    Value* execute(Array* node) override
    {
        ArrayValue* arr = new ArrayValue{};
        for(int i = 0; i < node->elements.size(); i++)
        {
            Value* value = unWrapValue<Value>(executeNode(node->elements[i]));
            std::unique_ptr<Value> element{value};
            arr->elements.push_back(std::move(element));
        }
        return arr;
    }

    Value* execute(String* node) override
    {
        ArrayValue* arr = new ArrayValue{};
        for(int i = 0; i < node->str.size(); i++)
        {
            std::unique_ptr<Value> element = std::make_unique<CharacterValue>(node->str[i]);
            arr->elements.push_back(std::move(element));
        }
        return arr;
    }

    Value* execute(VariableLengthArray* node) override
    {
        std::unique_ptr<IntegerValue> val{unWrapValue<IntegerValue>(executeNode(node->size))};                
        ArrayValue* arr = new ArrayValue{};

        arr->elements.resize(val->number);
        for(int i = 0; i < val->number; i++)
        {
            std::unique_ptr<Value> element{makeValue(node->token->type)};
            arr->elements[i] = std::move(element);
        }
        return arr;
    }
    
    Value* execute(Unary* node) override
    {
        Value* value = unWrapValue<Value>(executeNode(node->child));
        TokenType tokenType = node->token->type;
        std::unique_ptr<TypeClass>& nodeTypeClass = unwrapTypeClass(node->typeClass);

        if (checkType(nodeTypeClass, Type::INTEGER) && tokenType == TokenType::MINUS)
        {
            castValue<IntegerValue>(value)->number = -castValue<IntegerValue>(value)->number;
        }
        if (checkType(nodeTypeClass, Type::DOUBLE) && tokenType == TokenType::MINUS)
        {
            castValue<RealValue>(value)->number = -castValue<RealValue>(value)->number;
        }
        else if (tokenType == TokenType::NOT)
        {
            castValue<BoolValue>(value)->value = !castValue<BoolValue>(value)->value;
        }

        return value;
    }

    Value* arithmetic(ASTnode* lNode, ASTnode* rNode, Value* left, Value* right, TokenType type)
    {        
        Value* resValue = nullptr;
        std::unique_ptr<TypeClass>& lType = unwrapTypeClass(lNode->typeClass);
        std::unique_ptr<TypeClass>& rType = unwrapTypeClass(rNode->typeClass);
        
        if (checkType(lType, Type::DOUBLE, rType, Type::DOUBLE))
        {
            double a = castValue<RealValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            double res = arithmeticOp<double, double, double>(a, b, type);
            resValue = new RealValue{res};
        }
        else if (checkType(lType, Type::DOUBLE, rType, Type::INTEGER))
        {
            double a = castValue<RealValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            double res = arithmeticOp<double, int, double>(a, b, type);
            resValue = new RealValue{res};
        }
        else if (checkType(lType, Type::INTEGER, rType, Type::DOUBLE))
        {
            int a = castValue<IntegerValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            double res = arithmeticOp<int, double, double>(a, b, type);
            resValue = new RealValue{res};
        }
        else if (checkType(lType, Type::INTEGER, rType, Type::INTEGER))
        {
            int a = castValue<IntegerValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            int res = type == TokenType::PERCENT ? a % b : arithmeticOp<int, int, int>(a, b, type);
            resValue = new IntegerValue{res};
        }
        return resValue;
    }

    Value* execute(Term* node) override
    {
        Value* left = unWrapValue<Value>(executeNode(node->left));
        Value* right = unWrapValue<Value>(executeNode(node->right));

        ASTnode* leftNode = node->left.get();
        ASTnode* rightNode = node->right.get();
    
        TokenType tokenType = node->token->type;

        Value* resValue = arithmetic(leftNode, rightNode, left, right, tokenType);

        delete left;
        delete right;
        return resValue;
    }
    
    Value* execute(Factor* node) override
    {
        Value* left = unWrapValue<Value>(executeNode(node->left));
        Value* right = unWrapValue<Value>(executeNode(node->right));

        ASTnode* leftNode = node->left.get();
        ASTnode* rightNode = node->right.get();
    
        TokenType tokenType = node->token->type;

        Value* resValue = arithmetic(leftNode, rightNode, left, right, tokenType);

        delete left;
        delete right;
        return resValue;
    }

    Value* execute(Relational* node) override
    {
        Value* left = unWrapValue<Value>(executeNode(node->left));
        Value* right = unWrapValue<Value>(executeNode(node->right));

        std::unique_ptr<TypeClass>& leftType = unwrapTypeClass(node->left->typeClass);
        std::unique_ptr<TypeClass>& rightType = unwrapTypeClass(node->right->typeClass);

        TokenType tokenType = node->token->type;
        
        Value* resValue;
        if (checkType(leftType, Type::DOUBLE, rightType, Type::DOUBLE))
        {
            double a = castValue<RealValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            bool res = RelationalOp<double, double>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftType, Type::DOUBLE, rightType, Type::INTEGER))
        {
            double a = castValue<RealValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            bool res = RelationalOp<double, int>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftType, Type::INTEGER, rightType, Type::DOUBLE))
        {
            int a = castValue<IntegerValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            bool res = RelationalOp<int, double>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftType, Type::INTEGER, rightType, Type::INTEGER))
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
        Value* left = unWrapValue<Value>(executeNode(node->left));
        Value* right = unWrapValue<Value>(executeNode(node->right));

        std::unique_ptr<TypeClass>& leftType = unwrapTypeClass(node->left->typeClass);
        std::unique_ptr<TypeClass>& rightType = unwrapTypeClass(node->right->typeClass);

        TokenType tokenType = node->token->type;
        
        Value* resValue;
        if (checkType(leftType, Type::DOUBLE, rightType, Type::DOUBLE))
        {
            double a = castValue<RealValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            bool res = EquivalentOp<double, double>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftType, Type::DOUBLE, rightType, Type::INTEGER))
        {
            double a = castValue<RealValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            bool res = EquivalentOp<double, int>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftType, Type::INTEGER, rightType, Type::DOUBLE))
        {
            int a = castValue<IntegerValue>(left)->number;
            double b = castValue<RealValue>(right)->number;
            bool res = EquivalentOp<int, double>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftType, Type::INTEGER, rightType, Type::INTEGER))
        {
            int a = castValue<IntegerValue>(left)->number;
            int b = castValue<IntegerValue>(right)->number;
            bool res = EquivalentOp<int, int>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftType, Type::CHARACTER, rightType, Type::CHARACTER))
        {
            char a = castValue<CharacterValue>(left)->character;
            char b = castValue<CharacterValue>(right)->character;
            bool res = EquivalentOp<char, char>(a, b, tokenType);
            resValue = new BoolValue{res};
        }
        else if (checkType(leftType, Type::BOOLEAN, rightType, Type::BOOLEAN))
        {
            bool a = castValue<BoolValue>(left)->value;
            bool b = castValue<BoolValue>(right)->value;
            bool res = EquivalentOp<bool, bool>(a, b, tokenType);
            resValue = new BoolValue{res};
        }

        delete left;
        delete right;
        return resValue;
    }

    Value* And(Logical* node)
    {
        BoolValue* temp = unWrapValue<BoolValue>(executeNode(node->left));

        if (temp->value == false)
        {
            return temp;
        }
        else
        {
            delete temp;
            return unWrapValue<BoolValue>(executeNode(node->right));
        } 
    }
        
    Value* Or(Logical* node)
    {
        BoolValue* temp = unWrapValue<BoolValue>(executeNode(node->left));

        if (temp->value == true)
        {
            return temp;
        }
        else
        {
            delete temp;
            return unWrapValue<BoolValue>(executeNode(node->right));
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

    Value* execute(PrintStmt* node) override
    {
        Value* value = unWrapValue<Value>(executeNode(node->expr));
        
        switch(value->type)
        {
            case ValueType::INTEGER:
                std::cout<<castValue<IntegerValue>(value)->number;
                break;
            case ValueType::REAL:
                std::cout<<castValue<RealValue>(value)->number;
                break;
            case ValueType::CHAR:
                std::cout<<castValue<CharacterValue>(value)->character;
                break;
            case ValueType::BOOL:
                std::cout<<castValue<BoolValue>(value)->value;
                break;
            default:
                std::cout<<"Compound Type\n";
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

    Value* makeValue(std::unique_ptr<TypeClass>& typeClass)
    {
        switch (typeClass->baseType)
        {
            case Type::INTEGER:
                return new IntegerValue{0};
            case Type::DOUBLE:
                return new RealValue{0.0};
            case Type::BOOLEAN:
                return new BoolValue{false};
            case Type::CHARACTER:
                return new CharacterValue{'\0'};
            case Type::ARRAY:
                return new ArrayValue{};
            default:
                return nullptr;
        }
    }
    
    Value* makeValue(TokenType type)
    {
        switch (type)
        {
            case TokenType::INT:
                return new IntegerValue{0};
            case TokenType::REAL:
                return new RealValue{0.0};
            case TokenType::BOOL:
                return new BoolValue{false};
            case TokenType::CHAR:
                return new CharacterValue{'\0'};
            default:
                return nullptr;
        }
    }
    
    Value* execute(VarDecl* node) override
    {
        Value* valuePtr = node->expr ? unWrapValue(executeNode(node->expr))
                                     : makeValue(node->typeClass);

        std::unique_ptr<Value> value {valuePtr};
        stack.push_back(StackValue{node->name, std::move(value), currentScope});
        return nullptr;   
    }

    void copyAssignValue(Value* base, Value* value)
    {
        ValueType type = base->type;
        switch(type)
        {
            case ValueType::INTEGER:
                *castValue<IntegerValue>(base) = *castValue<IntegerValue>(value);
                break;
            case ValueType::REAL:
                *castValue<RealValue>(base) = *castValue<RealValue>(value);
                break;
            case ValueType::CHAR:
                *castValue<CharacterValue>(base) = *castValue<CharacterValue>(value);
                break;
            case ValueType::BOOL:
                *castValue<BoolValue>(base) = *castValue<BoolValue>(value);
                break;
            case ValueType::ARRAY:
            {
                ArrayValue* baseArr = castValue<ArrayValue>(base);
                ArrayValue* exprArr = castValue<ArrayValue>(value);
                baseArr->elements.clear();
                for (std::unique_ptr<Value>& i : exprArr->elements)
                {
                    std::unique_ptr<Value> element{copyValue(i.get())};
                    baseArr->elements.push_back(std::move(element));
                }
                break;   
            }
            default:
                break;
        }
    }
    
    Value* execute(SetVar* node) override
    {
        std::unique_ptr<Value> value{unWrapValue(executeNode(node->expr))};
        std::unique_ptr<ReferenceValue> ref{castValue<ReferenceValue>(executeNode(node->variable))};
        copyAssignValue(ref->val, value.get());
        return nullptr;
    }

    Value* execute(ElementAccess* node) override
    {
        std::unique_ptr<IntegerValue> index{unWrapValue<IntegerValue>(executeNode(node->index))};
        std::unique_ptr<ReferenceValue> ref{castValue<ReferenceValue>(executeNode(node->node))};
        ArrayValue* arr = castValue<ArrayValue>(ref->val);

        if (index->number >= arr->elements.size() || index->number < 0)
        {
            throw Error{node->token, "Index is out of range."};
        }
        
        return new ReferenceValue{arr->elements[index->number].get()};
    }
    
    virtual ASTvalue* execute(IfStatement* node) override
    {
        BoolValue* value = unWrapValue<BoolValue>(executeNode(node->condition));

        if (value->value)
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
        BoolValue* value = unWrapValue<BoolValue>(executeNode(node->condition));

        if (value->value)
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

    virtual ASTvalue* execute(ElseStatement* node) override
    {
        node->block->execute(this);
        return nullptr;
    }


    virtual ASTvalue* execute(WhileStatement* node) override
    {
        while(true)
        {
            std::unique_ptr<BoolValue> value{unWrapValue<BoolValue>(executeNode(node->condition))};
            if (value->value == false) break;

            node->block->execute(this);
        }        
        return nullptr;
    }

    virtual ASTvalue* execute(ForStatement* node) override
    {
        beginScope();

        delete executeNode(node->initializer);

        while(true && node->condition == nullptr)
        {
            node->block->execute(this);
            delete executeNode(node->updator);
        }
        
        while(true && node->condition)
        {
            std::unique_ptr<BoolValue> value{unWrapValue<BoolValue>(executeNode(node->condition))};
            if (value->value == false) break;
            
            node->block->execute(this);
            delete executeNode(node->updator);
        }
        
        endScope();        
        return nullptr;
    }
    
    virtual ASTvalue* execute(BlockStatement* node) override
    {
        beginScope();
        node->stmtList->execute(this);
        endScope();        
        return nullptr;
    }
         
};

#endif
