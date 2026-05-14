#include "ASTnode.h"

ASTvalue* AstnodeOperator::execute(ASTnode* node)
{
    switch (node->type)
    {
        case ASTnodeType::TermNode: return execute(dynamic_cast<TermNode*>(node));
        case ASTnodeType::FactorNode: return execute(dynamic_cast<FactorNode*>(node));
        case ASTnodeType::UnaryNode: return execute(dynamic_cast<UnaryNode*>(node));
        case ASTnodeType::PrimaryNode: return execute(dynamic_cast<PrimaryNode*>(node));
        case ASTnodeType::NumberNode: return execute(dynamic_cast<NumberNode*>(node));
    }

    std::cout<<"error\n";
    return nullptr;
}

TermNode::TermNode(ASTnode* _left, ASTnode* _right, Token* _token)
        : ASTnode(ASTnodeType::TermNode), left(_left), right(_right), token(_token) 
{
    
}

ASTvalue* TermNode::execute(AstnodeOperator* operation)
{
    return operation->execute(this);
}

FactorNode::FactorNode(ASTnode* _left, ASTnode* _right, Token* _token)
        : ASTnode(ASTnodeType::FactorNode), left(_left), right(_right), token(_token)
{
    
}

ASTvalue* FactorNode::execute(AstnodeOperator* operation)
{
    return operation->execute(this);
}  

UnaryNode::UnaryNode(ASTnode* _node, Token* _token) :
        ASTnode(ASTnodeType::UnaryNode), node(_node), token(_token)
{
    
}

ASTvalue* UnaryNode::execute(AstnodeOperator* operation)
{
    return operation->execute(this);
}  

PrimaryNode::PrimaryNode(ASTnode* _node, Token* _token)
        : ASTnode(ASTnodeType::PrimaryNode), node(_node), token(_token)
{
    
}

ASTvalue* PrimaryNode::execute(AstnodeOperator* operation)
{
    return operation->execute(this);
}  

NumberNode::NumberNode(double _num, Token* _token)
        : ASTnode(ASTnodeType::NumberNode), num(_num), token(_token)
{
    
}

ASTvalue* NumberNode::execute(AstnodeOperator* operation)
{
    return operation->execute(this);
}  
