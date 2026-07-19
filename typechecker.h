#ifndef typechecker_h
#define typechecker_h

#include "ASTnode.h"
#include "common.h"
#include "typeclass.h"

class TypeChecker : public AstnodeOperator
{
    struct StackValue
    {
        std::string name;
        int scope;
        std::unique_ptr<TypeClass> typeClass;    
    };
    
    public:
              
    std::vector<StackValue> stack;
    int currentScope = 0;
    bool hadError = false;
    TypeChecker() {}

     
    ASTvalue* execute(Program* node) override
    {
        try
        {
            node->node->execute(this);
            node->typeClass = std::make_unique<VoidType>();
        }
        catch(Error error)
        {
            hadError = true;
            error.print();
        }

        return nullptr;
    }
    
    ASTvalue* execute(Integer* node) override
    {
        node->typeClass = std::make_unique<IntegerType>();
        return nullptr;
    }
    
    ASTvalue* execute(Double* node) override
    {
        node->typeClass = std::make_unique<DoubleType>();
        return nullptr;
    }
    
    ASTvalue* execute(Character* node) override
    {
        node->typeClass = std::make_unique<CharacterType>();
        return nullptr;
    }
       
    ASTvalue* execute(Boolean* node) override
    {
        node->typeClass = std::make_unique<BooleanType>();
        return nullptr;
    }    

    ASTvalue* execute(String* node) override
    {
        node->typeClass = std::make_unique<ArrayType>(std::make_unique<CharacterType>());
        return nullptr;
    }    

    ASTvalue* execute(Array* node) override
    {
        node->elements[0]->execute(this);
        std::unique_ptr<TypeClass>& elementType = unWrapReference(node->elements[0]->typeClass);
        
        if(elementType->baseType == Type::ARRAY) throw Error{"Nested Arrays are not allowed."};

        for(int i = 1; i < node->elements.size(); i++)
        {
            node->elements[i]->execute(this);
            if(!isEqual(elementType, unWrapReference(node->elements[i]->typeClass)))
            {
                throw Error{"Arrays cannot be of mixed types."};
            }
        }

        node->typeClass = std::make_unique<ArrayType>(copyTypeClass(elementType));
        return nullptr;
    }

    ASTvalue* execute(VariableLengthArray* node) override
    {
        node->size->execute(this);
        if (unWrapReference(node->size->typeClass)->baseType != Type::INTEGER)
        {
            throw Error{"Index should be a integer."};    
        }
        
        node->typeClass = std::make_unique<ArrayType>(toTypeClass(node->token->type));
        return nullptr;
    }
    
    std::unique_ptr<TypeClass>& unWrapReference(std::unique_ptr<TypeClass>& typeClass)
    {
        if(typeClass->baseType == Type::REFERENCE)
        {
            return dynamic_cast<ReferenceType*>(typeClass.get())->simpleType;
        }

        return typeClass;
    }
    
    ASTvalue* execute(Unary* node) override
    {
        ASTnode* child = node->child.get();
        child->execute(this);

        std::unique_ptr<TypeClass>& childTypeClass = unWrapReference(child->typeClass);
        TokenType tokenType = node->token->type;
        
        if (tokenType == TokenType::MINUS && isType(childTypeClass, Type::INTEGER))
        {
            node->typeClass = std::make_unique<IntegerType>();
            return nullptr;
        }
        
        if (tokenType == TokenType::MINUS && isType(childTypeClass, Type::DOUBLE))
        {
            node->typeClass = std::make_unique<DoubleType>();
            return nullptr;
        }
        
        if (tokenType == TokenType::NOT && isType(childTypeClass, Type::BOOLEAN))
        {
            node->typeClass = std::make_unique<BooleanType>();
            return nullptr;
        }

        throw Error{node->token, "Operand type doesn't match with the operator."};
    }

    ASTvalue* execute(Term* node) override
    {
        node->left->execute(this);
        node->right->execute(this);
    
        Type leftType = unWrapReference(node->left->typeClass)->baseType;
        Type rightType = unWrapReference(node->right->typeClass)->baseType;

        if (leftType == Type::DOUBLE && rightType == Type::DOUBLE)
        {
            node->typeClass = std::make_unique<DoubleType>();          
            return nullptr;
        }
        
        if (leftType == Type::DOUBLE && rightType == Type::INTEGER)
        {
            node->typeClass = std::make_unique<DoubleType>();          
            return nullptr;
        }
        
        if (leftType == Type::INTEGER && rightType == Type::DOUBLE)
        {
            node->typeClass = std::make_unique<DoubleType>();          
            return nullptr;
        }
        
        if (leftType == Type::INTEGER && rightType == Type::INTEGER)
        {
            node->typeClass = std::make_unique<IntegerType>();          
            return nullptr;
        }
        
        throw Error{node->token, "Operand type doesn't match with the operator."};
    }

    ASTvalue* execute(Factor* node) override
    {
        node->left->execute(this);
        node->right->execute(this);

        Type leftType = unWrapReference(node->left->typeClass)->baseType;
        Type rightType = unWrapReference(node->right->typeClass)->baseType;
        bool isPercent = node->token->type == TokenType::PERCENT;

        if (leftType == Type::DOUBLE && rightType == Type::DOUBLE && !isPercent)
        {
            node->typeClass = std::make_unique<DoubleType>();          
            return nullptr;
        }
        
        if (leftType == Type::DOUBLE && rightType == Type::INTEGER && !isPercent)
        {
            node->typeClass = std::make_unique<DoubleType>();          
            return nullptr;
        }
        
        if (leftType == Type::INTEGER && rightType == Type::DOUBLE && !isPercent)
        {
            node->typeClass = std::make_unique<DoubleType>();          
            return nullptr;
        }
        
        if (leftType == Type::INTEGER && rightType == Type::INTEGER)
        {
            node->typeClass = std::make_unique<IntegerType>();          
            return nullptr;
        }
        
        throw Error{node->token, "Operand type doesn't match with the operator."};
    }
    
    ASTvalue* execute(Relational* node) override
    {
        node->left->execute(this);
        node->right->execute(this);

        Type leftType = unWrapReference(node->left->typeClass)->baseType;
        Type rightType = unWrapReference(node->right->typeClass)->baseType;

        if(isNumberType(leftType) && isNumberType(rightType))
        {
            node->typeClass = std::make_unique<BooleanType>();
            return nullptr;
        }

        throw Error{node->token, "Operand type doesn't match with the operator."};
    }

    ASTvalue* execute(Equivalent* node) override
    {
        node->left->execute(this);
        node->right->execute(this);

        Type left = unWrapReference(node->left->typeClass)->baseType;
        Type right = unWrapReference(node->right->typeClass)->baseType;

        if(isComparable(left, right))
        {
            node->typeClass = std::make_unique<BooleanType>();
            return nullptr;
        }
        
        throw Error{node->token, "Operand type doesn't match with the operator."};
    }
    
    ASTvalue* execute(Logical* node) override
    {
        node->left->execute(this);
        node->right->execute(this);

        Type left = unWrapReference(node->left->typeClass)->baseType;
        Type right = unWrapReference(node->right->typeClass)->baseType;

        if(checkType(left, Type::BOOLEAN, right, Type::BOOLEAN))
        {
            node->typeClass = std::make_unique<BooleanType>();
            return nullptr;
        }
        
        throw Error{node->token, "Operand type doesn't match with the operator."};
    }

    
    
    std::unique_ptr<TypeClass> copyTypeClass(std::unique_ptr<TypeClass>& typeClass)
    {
        TypeClass* typePtr = typeClass.get();
        switch (typeClass->baseType)
        {
            case Type::INTEGER:
                return std::make_unique<IntegerType>();
            case Type::DOUBLE:
                return std::make_unique<DoubleType>();
            case Type::BOOLEAN:
                return std::make_unique<BooleanType>();
            case Type::CHARACTER:
                return std::make_unique<CharacterType>();
            case Type::ARRAY:
            {
                ArrayType* arrType = dynamic_cast<ArrayType*>(typePtr);
                std::unique_ptr<TypeClass>& elementType = arrType->elementType;
                return std::make_unique<ArrayType>(std::move(copyTypeClass(elementType)));
            }
            case Type::REFERENCE:
            {
                ReferenceType* refType = dynamic_cast<ReferenceType*>(typePtr);
                std::unique_ptr<TypeClass>& simpleType = refType->simpleType;
                return std::make_unique<ReferenceType>(std::move(copyTypeClass(simpleType)));
            }
            case Type::VOID:
                return std::make_unique<VoidType>();
        }
        return nullptr;
    }
    
    std::unique_ptr<TypeClass> toTypeClass(TokenType tokenType)
    {
        switch (tokenType)
        {
            case TokenType::INT:
                return std::make_unique<IntegerType>();
            case TokenType::REAL:
                return std::make_unique<DoubleType>();
            case TokenType::BOOL:
                return std::make_unique<BooleanType>();
            case TokenType::CHAR:
                return std::make_unique<CharacterType>();
            default:
                return std::make_unique<VoidType>();
        }
    }
    
    bool isEqual(std::unique_ptr<TypeClass>& a, std::unique_ptr<TypeClass>& b)
    {
        if (a->baseType == Type::ARRAY && b->baseType == Type::ARRAY)
        {
            std::unique_ptr<TypeClass>& left = dynamic_cast<ArrayType*>(a.get())->elementType; 
            std::unique_ptr<TypeClass>& right = dynamic_cast<ArrayType*>(b.get())->elementType;
            return isEqual(left, right); 
        }
        
        return a->baseType == b->baseType;
    }

    ASTvalue* execute(VarDecl* node) override
    {
        std::unique_ptr<TypeClass> varTypeClass = toTypeClass(node->dataType->type);        
        if (node->isArray)
        {
            varTypeClass = std::make_unique<ArrayType>(std::move(varTypeClass));
        }
        node->typeClass = copyTypeClass(varTypeClass);

        if (node->expr)
        {
            node->expr->execute(this);
        }
        else
        {
            stack.push_back({node->name, currentScope, std::move(varTypeClass)});
            return nullptr;
        }

        std::unique_ptr<TypeClass>& exprTypeClass = unWrapReference(node->expr->typeClass);
        if (isEqual(varTypeClass, exprTypeClass))
        {
            stack.push_back({node->name, currentScope, std::move(varTypeClass)});
            return nullptr;
        }
        
        throw Error{node->dataType, "Cannot initialize a value of incompatible type."};
    }

    ASTvalue* execute(ElementAccess* node) override
    {
        node->index->execute(this);
        node->node->execute(this);
        TypeClass* varType = unWrapReference(node->node->typeClass).get();
        Type indexType = unWrapReference(node->index->typeClass)->baseType;
        
        if (indexType != Type::INTEGER)
        {
            throw Error{node->token, "Expected a integer type in array index."};    
        }

        if (varType->baseType != Type::ARRAY)
        {
            throw Error{node->token, "Indexing a variable that is not an array."};    
        }

        std::unique_ptr<TypeClass>& elementType = dynamic_cast<ArrayType*>(varType)->elementType;
        std::unique_ptr<TypeClass> copiedElementType = copyTypeClass(elementType);

        if (node->node->typeClass->baseType == Type::REFERENCE)
        {
            node->typeClass = std::make_unique<ReferenceType>(std::move(copiedElementType));
        }
        else
        {            
            node->typeClass = std::move(copiedElementType);
        }
        
        return nullptr;
    }
    
    ASTvalue* execute(SetVar* node) override
    {
        node->typeClass = std::make_unique<VoidType>();
        node->expr->execute(this);
        node->variable->execute(this);

        bool isReference = node->variable->typeClass->baseType == Type::REFERENCE;
        std::unique_ptr<TypeClass>& varTypeClass = unWrapReference(node->variable->typeClass);
        std::unique_ptr<TypeClass>& exprTypeClass = unWrapReference(node->expr->typeClass);

        if (isReference && isEqual(varTypeClass, exprTypeClass)) return nullptr;

        throw Error{node->token, "Assigning a value of incompatible type."};
    }

    ASTvalue* execute(StatementList* node) override
    {
        for(int i = 0; i < node->list.size(); i++)
        {
            node->list[i]->execute(this);    
        }
        
        node->typeClass = std::make_unique<VoidType>();
        return nullptr;
    }

    ASTvalue* execute(PrintStmt* node) override
    {
        node->expr->execute(this);
        node->typeClass = std::make_unique<VoidType>();
        return nullptr;
    }
    
    ASTvalue* execute(Name* node) override
    {          
        for (int i = stack.size() - 1; i >= 0; i--)
        {
            if (node->name == stack[i].name)
            {
                std::unique_ptr<TypeClass> baseType = copyTypeClass(stack[i].typeClass);

                std::unique_ptr<ReferenceType> refType;
                refType = std::make_unique<ReferenceType>(std::move(baseType));

                node->typeClass = std::move(refType);
                return nullptr;
            }
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
        
        node->typeClass = std::make_unique<VoidType>();
        return nullptr;
    }
     
    virtual ASTvalue* execute(IfStatement* node) override 
    {
        node->typeClass = std::make_unique<VoidType>();
        node->condition->execute(this);

        std::unique_ptr<TypeClass>& typeClass = unWrapReference(node->condition->typeClass);
        if (typeClass->baseType != Type::BOOLEAN)
        {
            throw Error{node->token, "Expected a boolean expression inside the condition."};
        }
        
        node->block->execute(this);
        if (node->tail) node->tail->execute(this);
        return nullptr;
    }
    
    virtual ASTvalue* execute(ElifStatement* node) override 
    {
        node->typeClass = std::make_unique<VoidType>();
        node->condition->execute(this);

        std::unique_ptr<TypeClass>& typeClass = unWrapReference(node->condition->typeClass);
        if (typeClass->baseType != Type::BOOLEAN)
        {
            throw Error{node->token, "Expected a boolean expression inside the condition."};
        }
        
        node->block->execute(this);
        if (node->tail) node->tail->execute(this);
        
        return nullptr;
    }

    virtual ASTvalue* execute(ElseStatement* node) override 
    {
        node->typeClass = std::make_unique<VoidType>();
        node->block->execute(this);
        return nullptr;
    }

    virtual ASTvalue* execute(WhileStatement* node) override 
    {
        node->typeClass = std::make_unique<VoidType>();
        node->condition->execute(this);

        std::unique_ptr<TypeClass>& typeClass = unWrapReference(node->condition->typeClass);
        if (typeClass->baseType != Type::BOOLEAN)
        {
            throw Error{node->token, "Expected a boolean expression inside the condition."};
        }
        
        node->block->execute(this);
       
        return nullptr;
    }

    virtual ASTvalue* execute(ForStatement* node) override
    {
        beginScope();        
        if (node->initializer) node->initializer->execute(this);
        if (node->condition) node->condition->execute(this);
        if (node->updator) node->updator->execute(this);
        node->block->execute(this);
        endScope();

        if (node->condition &&
            unWrapReference(node->condition->typeClass)->baseType != Type::BOOLEAN)
        {
            throw Error{node->token, "Expected a boolean expression inside the condition."};
        }
        
        node->typeClass = std::make_unique<VoidType>();
        return nullptr;
    }

    bool isType(std::unique_ptr<TypeClass>& typeClass, Type type)
    {
        return typeClass->baseType == type;
    }

    bool checkType(Type left, Type leftExpected, Type right, Type rightExpected)
    {
        return left == leftExpected && right == rightExpected;
    }
    
    bool isComparable(Type left, Type right)
    {
        if (checkType(left, Type::INTEGER, right, Type::INTEGER)
            || checkType(left, Type::DOUBLE, right, Type::DOUBLE)
            || checkType(left, Type::INTEGER, right, Type::DOUBLE)
            || checkType(left, Type::DOUBLE, right, Type::INTEGER)
            || checkType(left, Type::BOOLEAN, right, Type::BOOLEAN)
            || checkType(left, Type::CHARACTER, right, Type::CHARACTER))
        {
            return true;    
        }

        return false;
    }
    
    bool isNumberType(Type type)
    {
        if (type == Type::INTEGER || type == Type::DOUBLE) return true;
        return false;
    }

};

#endif
