#ifndef ASTnode_h
#define ASTnode_h

#include <memory>
#include "token.h"
#include "common.h"

class AstnodeOperator;

enum class ASTnodeType
{
    TermNode,
    FactorNode,
    UnaryNode,
    PrimaryNode,
    NumberNode,
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

class TermNode;
class FactorNode;
class UnaryNode;
class PrimaryNode;
class NumberNode;

class AstnodeOperator
{
    protected:
    AstnodeOperator(){}

    public:
    virtual ASTvalue* execute(ASTnode* node);
    virtual ASTvalue* execute(TermNode* node) = 0;
    virtual ASTvalue* execute(FactorNode* node) = 0;
    virtual ASTvalue* execute(UnaryNode* node) = 0;
    virtual ASTvalue* execute(PrimaryNode* node) = 0;
    virtual ASTvalue* execute(NumberNode* node) = 0;    
};

class TermNode : public ASTnode
{
    public:
    ASTnodeType type = ASTnodeType::TermNode;
    std::unique_ptr<ASTnode> left;
    std::unique_ptr<ASTnode> right;
    Token* token;
    
    TermNode(ASTnode* _left, ASTnode* _right, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class FactorNode : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> left;
    std::unique_ptr<ASTnode> right;
    Token* token;
    
    FactorNode(ASTnode* _left, ASTnode* _right, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class UnaryNode : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> node;
    Token* token;
    
    UnaryNode(ASTnode* _node, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class PrimaryNode : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> node;
    Token* token;
    
    PrimaryNode(ASTnode* _node, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class NumberNode : public ASTnode
{
    public:
    double num;
    Token* token;
    
    NumberNode(double _num, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

#endif
