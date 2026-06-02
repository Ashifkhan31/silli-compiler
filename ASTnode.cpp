#include "ASTnode.h"

Integer::Integer(int _number, Token* _token) : number(_number), token(_token)
{
    
}

ASTvalue* Integer::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Integer*>(this));
}

Double::Double(double _number, Token* _token) : number(_number), token(_token)
{
    
}

ASTvalue* Double::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Double*>(this));
}

Character::Character(char _c, Token* _token) : c(_c), token(_token)
{
    
}

ASTvalue* Character::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Character*>(this));
}

String::String(std::string _str, Token* _token) : str(_str), token(_token)
{
    
}

ASTvalue* String::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<String*>(this));
}

Boolean::Boolean(bool _value, Token* _token) : value(_value), token(_token)
{
    
}

ASTvalue* Boolean::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Boolean*>(this));
}

Unary::Unary(ASTnode* _child, Token* _token) : child(_child), token(_token)
{
    
}

ASTvalue* Unary::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Unary*>(this));
}

Factor::Factor(ASTnode* _left, ASTnode* _right, Token* _token)
               : left(_left), right(_right), token(_token)
{
    
}

ASTvalue* Factor::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Factor*>(this));
}

Term::Term(ASTnode* _left, ASTnode* _right, Token* _token)
           : left(_left), right(_right), token(_token)
{
    
}

ASTvalue* Term::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Term*>(this));
}

Relational::Relational(ASTnode* _left, ASTnode* _right, Token* _token)
           : left(_left), right(_right), token(_token)
{
    
}

ASTvalue* Relational::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Relational*>(this));
}

Equivalent::Equivalent(ASTnode* _left, ASTnode* _right, Token* _token)
           : left(_left), right(_right), token(_token)
{
    
}

ASTvalue* Equivalent::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Equivalent*>(this));
}

Logical::Logical(ASTnode* _left, ASTnode* _right, Token* _token)
           : left(_left), right(_right), token(_token)
{
    
}

ASTvalue* Logical::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Logical*>(this));
}

Program::Program(ASTnode* _node) : node(_node)
{
    
}

ASTvalue* Program::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Program*>(this));
}

VarDecl::VarDecl(Token* _dataType, std::string _name, ASTnode* _expr, Token* _token) :
                 dataType(_dataType), name(_name), expr(_expr), token(_token)
{
    
}

ASTvalue* VarDecl::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<VarDecl*>(this));
}

SetVar::SetVar(std::string _name, ASTnode* _expr, Token* _token)
                : name(_name), expr(_expr), token(_token)
{
    
}

ASTvalue* SetVar::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<SetVar*>(this));
}

StatementList::StatementList()
{
    list.reserve(30);
}

ASTvalue* StatementList::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<StatementList*>(this));
}

Name::Name(std::string _name, Token* _token) :  name(_name), token(_token)
{
    
}

ASTvalue* Name::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Name*>(this));
}

PrintStmt::PrintStmt(ASTnode* _expr) : expr(_expr)
{
    
}

ASTvalue* PrintStmt::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<PrintStmt*>(this));
}
