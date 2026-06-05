#ifndef resolver_h
#define resolver_h

#include "common.h"
#include "ASTnode.h"

class Resolver : public AstnodeOperator
{
    struct StackValue
    {
        std::string name;
        int scope;    
    };
    
    public:
    std::vector<StackValue> stack;
    int currentScope = 0;
    bool hadError = false;
    
    virtual ASTvalue* execute(Program* node)
    {
        try
        {            
            node->node->execute(this);
        }
        catch(Error error)
        {
            hadError = true;
            error.print();
        }

        return nullptr;
    }

    virtual ASTvalue* execute(StatementList* node)
    {
        for(int i = 0; i < node->list.size(); i++)
        {
            node->list[i]->execute(this);    
        }
        
        return nullptr;
    }

    virtual ASTvalue* execute(Integer* node) {return nullptr;}
    virtual ASTvalue* execute(Double* node) {return nullptr;}
    virtual ASTvalue* execute(Character* node) {return nullptr;}
    virtual ASTvalue* execute(String* node) {return nullptr;}
    virtual ASTvalue* execute(Boolean* node) {return nullptr;}

    virtual ASTvalue* execute(Unary* node)
    {
        node->child->execute(this);
        return nullptr;    
    }
    
    virtual ASTvalue* execute(Factor* node)
    {
        node->left->execute(this);
        node->right->execute(this);
        return nullptr;    
    }
    
    virtual ASTvalue* execute(Term* node)
    {
        node->left->execute(this);
        node->right->execute(this);
        return nullptr;    
    }
    
    virtual ASTvalue* execute(Relational* node)
    {
        node->left->execute(this);
        node->right->execute(this);
        return nullptr;    
    }
    
    virtual ASTvalue* execute(Equivalent* node)
    {
        node->left->execute(this);
        node->right->execute(this);
        return nullptr;    
    }
    
    virtual ASTvalue* execute(Logical* node)
    {
        node->left->execute(this);
        node->right->execute(this);
        return nullptr;    
    }
    
    virtual ASTvalue* execute(PrintStmt* node)
    {
        node->expr->execute(this);
        return nullptr;
    }

    virtual ASTvalue* execute(VarDecl* node)
    {
        if (node->expr) node->expr->execute(this);
       
        for (int i = stack.size() - 1; i >= 0; i--)
        {
            if (stack[i].scope < currentScope) break;

            if (node->name == stack[i].name)
            {
                throw Error{node->token, "Redefinition of variable is not allowed."};    
            }
        }

        stack.push_back(StackValue{node->name, currentScope});
        return nullptr;
    }
    
    virtual ASTvalue* execute(SetVar* node)
    {
        node->expr->execute(this);
        
        for (int i = stack.size() - 1; i >= 0; i--)
        {
            if (node->name == stack[i].name)
            {
                return nullptr;
            }
        }

        throw Error{node->token, "Reference to undefined variable."};    
    }

    virtual ASTvalue* execute(Name* node)
    {
        for (int i = stack.size() - 1; i >= 0; i--)
        {
            if (node->name == stack[i].name)
            {
                return nullptr;
            }
        }

        throw Error{node->token, "Reference to undefined variable."};    
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
    
    virtual ASTvalue* execute(BlockStatement* node) 
    {
        beginScope();
        node->stmtList->execute(this);
        endScope();
        return nullptr;
    }
    
    virtual ASTvalue* execute(IfStatement* node) 
    {
        node->condition->execute(this);
        node->block->execute(this);
        if (node->tail) node->tail->execute(this);
        return nullptr;
    }

    virtual ASTvalue* execute(ElifStatement* node) 
    {
        node->condition->execute(this);
        node->block->execute(this);
        if (node->tail) node->tail->execute(this);
        return nullptr;
    }

    virtual ASTvalue* execute(ElseStatement* node) 
    {
        node->block->execute(this);
        return nullptr;
    }
    
    virtual ASTvalue* execute(WhileStatement* node) 
    {
        node->condition->execute(this);
        node->block->execute(this);
        return nullptr;
    }

    virtual ASTvalue* execute(ForStatement* node) 
    {
        beginScope();
                
        if (node->initializer) node->initializer->execute(this);
        if (node->condition) node->condition->execute(this);
        if (node->updator) node->updator->execute(this);
        node->block->execute(this);

        endScope();
        return nullptr;
    }
};

#endif
