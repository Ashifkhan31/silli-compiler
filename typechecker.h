#ifndef typechecker_h
#define typechecker_h

#include "ASTnode.h"

class TypeChecker : public AstnodeOperator
{
    public:
      
    bool hadError = false;
    TypeChecker() {}
    
    using AstnodeOperator::execute;
    
    virtual ASTvalue* execute(Integer* node)
    {
        node->exprType = ExprType::INTEGER;
        return nullptr;
    }
    
    virtual ASTvalue* execute(Double* node)
    {
        node->exprType = ExprType::DOUBLE;
        return nullptr;
    }
    
    virtual ASTvalue* execute(Character* node)
    {
        node->exprType = ExprType::CHARACTER;
        return nullptr;
    }
    
    virtual ASTvalue* execute(String* node)
    {
        node->exprType = ExprType::STRING;
        return nullptr;
    }
    
    virtual ASTvalue* execute(Boolean* node)
    {
        node->exprType = ExprType::BOOLEAN;
        return nullptr;
    }
    
    virtual ASTvalue* execute(Unary* node)
    {
        ASTnode* child = node->child.get();
        child->execute(this);
        TokenType tokenType = node->token->type;
        
        if (tokenType == TokenType::MINUS &&
            child->exprType == ExprType::INTEGER)
        {
            node->exprType = ExprType::INTEGER;
            return nullptr;
        }
        
        if (tokenType == TokenType::MINUS &&
            child->exprType == ExprType::DOUBLE)
        {
            node->exprType = ExprType::DOUBLE;
            return nullptr;
        }
        
        if (tokenType == TokenType::NOT &&
            child->exprType == ExprType::BOOLEAN)
        {
            node->exprType = ExprType::BOOLEAN;
            return nullptr;
        }

        std::cout<<"Operand type doesn't match with the operator at line "
                 <<node->token->line<<".\n";

        node->exprType = ExprType::VOID;
        hadError = true;
        return nullptr;
    }
};

#endif
