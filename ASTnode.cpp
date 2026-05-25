#include "ASTnode.h"

ASTvalue* AstnodeOperator::execute(ASTnode* node)
{
    switch (node->type)
    {
        case ASTnodeType::INTEGER:
          return execute(dynamic_cast<Integer*>(node));
        case ASTnodeType::DOUBLE:
          return execute(dynamic_cast<Double*>(node));
        case ASTnodeType::CHARACTER:
          return execute(dynamic_cast<Character*>(node));
        case ASTnodeType::STRING:
          return execute(dynamic_cast<String*>(node));
        case ASTnodeType::BOOLEAN:
          return execute(dynamic_cast<Boolean*>(node));
        case ASTnodeType::UNARY:
          return execute(dynamic_cast<Unary*>(node));
        case ASTnodeType::FACTOR:
          return execute(dynamic_cast<Factor*>(node));
    }

    return nullptr;
}

Integer::Integer(int _number, Token* _token)
            : ASTnode(ASTnodeType::INTEGER), number(_number), token(_token)
{
    
}

ASTvalue* Integer::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Integer*>(this));
}

Double::Double(double _number, Token* _token)
            : ASTnode(ASTnodeType::DOUBLE), number(_number), token(_token)
{
    
}

ASTvalue* Double::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Double*>(this));
}

Character::Character(char _c, Token* _token)
            : ASTnode(ASTnodeType::CHARACTER), c(_c), token(_token)
{
    
}

ASTvalue* Character::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Character*>(this));
}

String::String(std::string _str, Token* _token)
            : ASTnode(ASTnodeType::STRING), str(_str), token(_token)
{
    
}

ASTvalue* String::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<String*>(this));
}

Boolean::Boolean(bool _value, Token* _token)
            : ASTnode(ASTnodeType::BOOLEAN), value(_value), token(_token)
{
    
}

ASTvalue* Boolean::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Boolean*>(this));
}

Unary::Unary(ASTnode* _child, Token* _token)
            : ASTnode(ASTnodeType::UNARY), child(_child), token(_token)
{
    
}

ASTvalue* Unary::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Unary*>(this));
}

Factor::Factor(ASTnode* _left, ASTnode* _right, Token* _token)
      : ASTnode(ASTnodeType::FACTOR), left(_left), right(_right), token(_token)
{
    
}

ASTvalue* Factor::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Factor*>(this));
}
