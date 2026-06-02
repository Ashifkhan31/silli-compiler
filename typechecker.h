#ifndef typechecker_h
#define typechecker_h

#include "ASTnode.h"
#include "common.h"

class TypeChecker : public AstnodeOperator
{
    struct StackValue
    {
        std::string name;
        int scope;
        ExprType Exprtype;    
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

        throw Error{node->token, "Operand type doesn't match with the operator."};
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
        
        throw Error{node->token, "Operand type doesn't match with the operator."};
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
        
        throw Error{node->token, "Operand type doesn't match with the operator."};
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

        throw Error{node->token, "Operand type doesn't match with the operator."};
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
        
        throw Error{node->token, "Operand type doesn't match with the operator."};
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
        
        throw Error{node->token, "Operand type doesn't match with the operator."};
    }

    ExprType toExprType(TokenType type)
    {
        switch(type)
        {
            case TokenType::INT:
                return ExprType::INTEGER;
            case TokenType::CHAR: 
                return ExprType::CHARACTER;
            case TokenType::BOOL: 
                return ExprType::BOOLEAN;
            case TokenType::REAL: 
                return ExprType::DOUBLE;
            default:
                break;
        }
        return ExprType::VOID;
    }
    
    ASTvalue* execute(VarDecl* node) override
    {
        TokenType varType = node->dataType->type;
        stack.push_back(StackValue{node->name, currentScope, toExprType(varType)});

        if(node->expr == nullptr) return nullptr;
        
        node->expr->execute(this);
        ExprType exprType = node->expr->exprType;
        
        if (varType == TokenType::INT && exprType == ExprType::INTEGER)
        {
            return nullptr;
        }
        if (varType == TokenType::REAL && exprType == ExprType::DOUBLE)
        {
            return nullptr;
        }
        if (varType == TokenType::CHAR && exprType == ExprType::CHARACTER)
        {
            return nullptr;
        }
        if (varType == TokenType::BOOL && exprType == ExprType::BOOLEAN)
        {
            return nullptr;
        }
        
        throw Error{node->dataType, "Cannot initialize a value of incompatible type."};
    }

    ASTvalue* execute(SetVar* node) override
    {
        node->expr->execute(this);
        
        for (int i = stack.size() - 1; i >= 0; i--)
        {
            if (node->name == stack[i].name)
            {
                if (stack[i].Exprtype == node->expr->exprType) return nullptr;
                throw Error{node->token, "Assigning a value of incompatible type."};
            }
        }

        return nullptr;
    }

    ASTvalue* execute(StatementList* node) override
    {
        for(int i = 0; i < node->list.size(); i++)
        {
            node->list[i]->execute(this);    
        }
        
        node->exprType = ExprType::VOID;
        return nullptr;
    }

    ASTvalue* execute(PrintStmt* node) override
    {
        node->expr->execute(this);
        node->exprType = ExprType::VOID;
        return nullptr;
    }
    
    ASTvalue* execute(Name* node) override
    {          
        for (int i = stack.size() - 1; i >= 0; i--)
        {
            if (node->name == stack[i].name)
            {
                node->exprType = stack[i].Exprtype;
                return nullptr;
            }
        }

        return nullptr;
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
