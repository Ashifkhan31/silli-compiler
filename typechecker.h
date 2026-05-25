#ifndef typechecker_h
#define typechecker_h

#include "ASTnode.h"
#include "common.h"

class TypeChecker : public AstnodeOperator
{
    public:
      
    bool hadError = false;
    TypeChecker() {}

     
    ASTvalue* execute(ASTnode* node) override
    {
        try
        {
            return AstnodeOperator::execute(node);
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
        node->exprType = ExprType::INTEGER;
        return nullptr;
    }
    
    ASTvalue* execute(Double* node) override
    {
        node->exprType = ExprType::DOUBLE;
        return nullptr;
    }
    
    ASTvalue* execute(Character* node) override
    {
        node->exprType = ExprType::CHARACTER;
        return nullptr;
    }
    
    ASTvalue* execute(String* node) override
    {
        node->exprType = ExprType::STRING;
        return nullptr;
    }
    
    ASTvalue* execute(Boolean* node) override
    {
        node->exprType = ExprType::BOOLEAN;
        return nullptr;
    }    
    
    ASTvalue* execute(Unary* node) override
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

        throw Error{"Operand type doesn't match with the operatoe [line "
                    + std::to_string(node->token->line)
                    + "]."};
    }

    ASTvalue* execute(Factor* node) override
    {
        node->left->execute(this);
        node->right->execute(this);

        ExprType leftExprType = node->left->exprType;
        ExprType rightExprType = node->right->exprType;
        bool isPercent = node->token->type == TokenType::PERCENT;

        if (leftExprType == ExprType::DOUBLE &&
            rightExprType == ExprType::DOUBLE && !isPercent)
        {
            node->exprType = ExprType::DOUBLE;          
            return nullptr;
        }
        
        if (leftExprType == ExprType::DOUBLE &&
            rightExprType == ExprType::INTEGER && !isPercent)
        {
            node->exprType = ExprType::DOUBLE;          
            return nullptr;
        }
        
        if (leftExprType == ExprType::INTEGER &&
            rightExprType == ExprType::DOUBLE && !isPercent)
        {
            node->exprType = ExprType::DOUBLE;          
            return nullptr;
        }
        
        if (leftExprType == ExprType::INTEGER &&
            rightExprType == ExprType::INTEGER)
        {
            node->exprType = ExprType::INTEGER;          
            return nullptr;
        }
        
        throw Error{"Operand type doesn't match with the operator [line "
                    + std::to_string(node->token->line)
                    + "]."};
    }
};

#endif
