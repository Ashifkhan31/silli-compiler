#ifndef ASTnode_h
#define ASTnode_h

#include <memory>
#include "token.h"
#include "common.h"

class AstnodeOperator;

enum class ASTnodeType
{
};

class ASTvalue
{
    protected:
    ASTvalue() {};

    public:
    virtual ~ASTvalue() {};
};

class ASTnode
{
    public:
    ASTnodeType type;
    virtual ASTvalue* execute(AstnodeOperator* operation) = 0;

    protected:
    ASTnode(ASTnodeType _type) : type(_type) {}
};

class AstnodeOperator
{
    protected:
    AstnodeOperator(){}

    public:
    virtual ASTvalue* execute(ASTnode* node);
};

#endif
