#ifndef typechecker_h
#define typechecker_h

#include "ASTnode.h"
#include "common.h"

class TypeChecker : public AstnodeOperator
{
    public:
      
    bool hadError = false;
    TypeChecker() {}

     
    ASTvalue* execute(Program* node) override
    {
        try
        {
            node->node->execute(this);
            node->exprType = ExprType::VOID;
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
        
        if (tokenType == TokenType::MINUS && child->exprType == ExprType::INTEGER)
        {
            node->exprType = ExprType::INTEGER;
            return nullptr;
        }
        
        if (tokenType == TokenType::MINUS && child->exprType == ExprType::DOUBLE)
        {
            node->exprType = ExprType::DOUBLE;
            return nullptr;
        }
        
        if (tokenType == TokenType::NOT && child->exprType == ExprType::BOOLEAN)
        {
            node->exprType = ExprType::BOOLEAN;
            return nullptr;
        }

        throw Error{"Operand type doesn't match with the operatoe [line "
                    + std::to_string(node->token->line)
                    + "]."};
    }

    ASTvalue* execute(Term* node) override
    {
        node->left->execute(this);
        node->right->execute(this);

        ExprType leftType = node->left->exprType;
        ExprType rightType = node->right->exprType;

        if (leftType == ExprType::DOUBLE && rightType == ExprType::DOUBLE)
        {
            node->exprType = ExprType::DOUBLE;          
            return nullptr;
        }
        
        if (leftType == ExprType::DOUBLE && rightType == ExprType::INTEGER)
        {
            node->exprType = ExprType::DOUBLE;          
            return nullptr;
        }
        
        if (leftType == ExprType::INTEGER && rightType == ExprType::DOUBLE)
        {
            node->exprType = ExprType::DOUBLE;          
            return nullptr;
        }
        
        if (leftType == ExprType::INTEGER && rightType == ExprType::INTEGER)
        {
            node->exprType = ExprType::INTEGER;          
            return nullptr;
        }
        
        throw Error{"Operand type doesn't match with the operator [line "
                    + std::to_string(node->token->line)
                    + "]."};
    }

    ASTvalue* execute(Factor* node) override
    {
        node->left->execute(this);
        node->right->execute(this);

        ExprType leftType = node->left->exprType;
        ExprType rightType = node->right->exprType;
        bool isPercent = node->token->type == TokenType::PERCENT;

        if (leftType == ExprType::DOUBLE && rightType == ExprType::DOUBLE && !isPercent)
        {
            node->exprType = ExprType::DOUBLE;          
            return nullptr;
        }
        
        if (leftType == ExprType::DOUBLE && rightType == ExprType::INTEGER && !isPercent)
        {
            node->exprType = ExprType::DOUBLE;          
            return nullptr;
        }
        
        if (leftType == ExprType::INTEGER && rightType == ExprType::DOUBLE && !isPercent)
        {
            node->exprType = ExprType::DOUBLE;          
            return nullptr;
        }
        
        if (leftType == ExprType::INTEGER && rightType == ExprType::INTEGER)
        {
            node->exprType = ExprType::INTEGER;          
            return nullptr;
        }
        
        throw Error{"Operand type doesn't match with the operator [line "
                    + std::to_string(node->token->line)
                    + "]."};
    }
    
    ASTvalue* execute(Relational* node) override
    {
        node->left->execute(this);
        node->right->execute(this);

        ExprType leftType = node->left->exprType;
        ExprType rightType = node->right->exprType;

        if(isNumberType(leftType) && isNumberType(rightType))
        {
            node->exprType = ExprType::BOOLEAN;
            return nullptr;
        }
        
        throw Error{"Operand type doesn't match with the operator [line "
                    + std::to_string(node->token->line)
                    + "]."};
    }

    ASTvalue* execute(Equivalent* node) override
    {
        node->left->execute(this);
        node->right->execute(this);

        ExprType left = node->left->exprType;
        ExprType right = node->right->exprType;

        if(isCompatible(left, right) && isComparable(left) && isComparable(right))
        {
            node->exprType = ExprType::BOOLEAN;
            return nullptr;
        }
        
        throw Error{"Operand type doesn't match with the operator [line "
                    + std::to_string(node->token->line)
                    + "]."};
    }
    
    ASTvalue* execute(Logical* node) override
    {
        node->left->execute(this);
        node->right->execute(this);

        ExprType left = node->left->exprType;
        ExprType right = node->right->exprType;

        if(checkType(left, ExprType::BOOLEAN, right, ExprType::BOOLEAN))
        {
            node->exprType = ExprType::BOOLEAN;
            return nullptr;
        }
        
        throw Error{"Operand type doesn't match with the operator [line "
                    + std::to_string(node->token->line)
                    + "]."};
    }
    
    bool checkType(ExprType left, ExprType leftExpected, ExprType right, ExprType rightExpected)
    {
        return left == leftExpected && right == rightExpected;
    }

    bool isComparable(ExprType expr)
    {
        if (expr == ExprType::INTEGER || expr == ExprType::DOUBLE ||
            expr == ExprType::BOOLEAN || expr == ExprType::CHARACTER)
        {
            return true;    
        }

        return false;
    }
    
    bool isCompatible(ExprType left, ExprType right)
    {
        if (left == right
            || checkType(left, ExprType::INTEGER, right, ExprType::DOUBLE)
            || checkType(left, ExprType::DOUBLE, right, ExprType::INTEGER))
        {
            return true;    
        }

        return false;
    }
    
    bool isNumberType(ExprType expr)
    {
        if (expr == ExprType::INTEGER || expr == ExprType::DOUBLE) return true;
        return false;
    }

};

#endif
