#include "ASTnode.h"

Program::Program(std::unique_ptr<ASTnode> _node, Token* _token)
                : ASTnode(_token), node(std::move(_node))
{
    
}

std::unique_ptr<ASTvalue> Program::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Program*>(this));
}

Int::Int(Token* _token) : ASTnode(_token)
{
    
}

std::unique_ptr<ASTvalue> Int::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Int*>(this));
}

Real::Real(Token* _token) : ASTnode(_token)
{
    
}

std::unique_ptr<ASTvalue> Real::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Real*>(this));
}

Char::Char(Token* _token) : ASTnode(_token)
{
    
}

std::unique_ptr<ASTvalue> Char::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Char*>(this));
}

Bool::Bool(Token* _token) : ASTnode(_token)
{
    
}

std::unique_ptr<ASTvalue> Bool::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Bool*>(this));
}

TypeName::TypeName(std::string _name, Token* _token) : ASTnode(_token), name(_name)
{
    
}

std::unique_ptr<ASTvalue> TypeName::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<TypeName*>(this));
}

FnType::FnType(std::unique_ptr<ASTnode> _returnType,
               std::vector<std::unique_ptr<ASTnode>> _paramTypes, Token* _token)
               : ASTnode(_token), returnType(std::move(_returnType)),
                 paramTypes(std::move(_paramTypes)) 
{
    
}

std::unique_ptr<ASTvalue> FnType::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<FnType*>(this));
}

IsArray::IsArray(std::unique_ptr<ASTnode> _type, Token* _token)
                : ASTnode(_token), type(std::move(_type))
{
    
}

std::unique_ptr<ASTvalue> IsArray::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<IsArray*>(this));
}

IsReference::IsReference(std::unique_ptr<ASTnode> _type,
                         Token* _token)
                         :
                         ASTnode(_token),
                         type(std::move(_type))
{
    
}

std::unique_ptr<ASTvalue> IsReference::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<IsReference*>(this));
}

Integer::Integer(int _number, Token* _token) : ASTnode(_token), number(_number)
{
    
}

std::unique_ptr<ASTvalue> Integer::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Integer*>(this));
}

Double::Double(double _number, Token* _token) : ASTnode(_token), number(_number)
{
    
}

std::unique_ptr<ASTvalue> Double::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Double*>(this));
}

Character::Character(char _c, Token* _token) : ASTnode(_token), c(_c)
{
    
}

std::unique_ptr<ASTvalue> Character::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Character*>(this));
}

Boolean::Boolean(bool _truthValue, Token* _token) : ASTnode(_token), truthValue(_truthValue)
{
    
}

std::unique_ptr<ASTvalue> Boolean::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Boolean*>(this));
}

Array::Array(std::vector<std::unique_ptr<ASTnode>> _elements,
             Token* _token)
             :
             ASTnode(_token),
             elements(std::move(_elements))
{
    
}

std::unique_ptr<ASTvalue> Array::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Array*>(this));
}

String::String(std::string _str, Token* _token) : ASTnode(_token), str(_str) 
{
    
}

std::unique_ptr<ASTvalue> String::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<String*>(this));
}

ArrayAllocation::ArrayAllocation(std::unique_ptr<ASTnode> _type,
                                 std::unique_ptr<ASTnode> _size,
                                 Token* _token)
                                 :
                                 ASTnode(_token),
                                 type(std::move(_type)),
                                 size(std::move(_size)) 
{
    
}

std::unique_ptr<ASTvalue> ArrayAllocation::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<ArrayAllocation*>(this));
}

StructValue::StructValue(std::string _structName,
                         std::vector<std::unique_ptr<ASTnode>> _elements,
                         Token* _token)
                         :
                         ASTnode(_token),
                         structName(_structName),
                         elements(std::move(_elements)) 
{
    
}

std::unique_ptr<ASTvalue> StructValue::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<StructValue*>(this));
}

Identifier::Identifier(std::string _name, Token* _token) : ASTnode(_token), name(_name) 
{
    
}

std::unique_ptr<ASTvalue> Identifier::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Identifier*>(this));
}

ArrayElementAccess::ArrayElementAccess(std::unique_ptr<ASTnode> _array,
                                       std::unique_ptr<ASTnode> _index,
                                       Token* _token)
                                       :
                                       ASTnode(_token),
                                       array(std::move(_array)),
                                       index(std::move(_index)) 
{
    
}

std::unique_ptr<ASTvalue> ArrayElementAccess::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<ArrayElementAccess*>(this));
}

StructElementAccess::StructElementAccess(std::unique_ptr<ASTnode> _base, std::string _name,
                                         Token* _token)
                                         : ASTnode(_token), base(std::move(_base)), name(_name)
{
    
}

std::unique_ptr<ASTvalue> StructElementAccess::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<StructElementAccess*>(this));
}

Call::Call(std::unique_ptr<ASTnode> _base, std::vector<std::unique_ptr<ASTnode>> _args,
           Token* _token)
           : ASTnode(_token), base(std::move(_base)), args(std::move(_args))
{
    
}

std::unique_ptr<ASTvalue> Call::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Call*>(this));
}

BinaryOperation::BinaryOperation(std::unique_ptr<ASTnode> _left, std::unique_ptr<ASTnode> _right,
                                 Token* _token)
                                 : ASTnode(_token), left(std::move(_left)), right(std::move(_right))
                                  
{
    
}

std::unique_ptr<ASTvalue> BinaryOperation::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<BinaryOperation*>(this));
}

UnaryOperation::UnaryOperation(std::unique_ptr<ASTnode> _value, Token* _token)
                               : ASTnode(_token), value(std::move(_value)) 
{
    
}

std::unique_ptr<ASTvalue> UnaryOperation::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<UnaryOperation*>(this));
}

VarDecl::VarDecl(std::unique_ptr<ASTnode> _type, std::string _name, std::unique_ptr<ASTnode> _expr,
                 Token* _token)
                 : ASTnode(_token), type(std::move(_type)), name(_name), expr(std::move(_expr)) 
{
    
}

std::unique_ptr<ASTvalue> VarDecl::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<VarDecl*>(this));
}

AssignmentStatement::AssignmentStatement(std::unique_ptr<ASTnode> _variable,
                                         std::unique_ptr<ASTnode> _expr, Token* _token)
                                         : ASTnode(_token), variable(std::move(_variable)),
                                           expr(std::move(_expr)) 
{
    
}

std::unique_ptr<ASTvalue> AssignmentStatement::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<AssignmentStatement*>(this));
}

ExpressionStatement::ExpressionStatement(std::unique_ptr<ASTnode> _expr, Token* _token)
                                         : ASTnode(_token), expr(std::move(_expr)) 
{
    
}

std::unique_ptr<ASTvalue> ExpressionStatement::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<ExpressionStatement*>(this));
}

StatementList::StatementList(std::vector<std::unique_ptr<ASTnode>> _statements, Token* _token)
                             : ASTnode(_token), statements(std::move(_statements)) 
{
    
}

std::unique_ptr<ASTvalue> StatementList::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<StatementList*>(this));
}

Block::Block(std::unique_ptr<ASTnode> _list, Token* _token)
             : ASTnode(_token), list(std::move(_list)) 
{
    
}

std::unique_ptr<ASTvalue> Block::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Block*>(this));
}

Parameter::Parameter(std::unique_ptr<ASTnode> _type, std::string _name, Token* _token)
                     : ASTnode(_token), type(std::move(_type)), name(_name) 
{
    
}

std::unique_ptr<ASTvalue> Parameter::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<Parameter*>(this));
}

FunDeclaration::FunDeclaration(std::string _name, std::vector<std::unique_ptr<ASTnode>> _params,
                               std::unique_ptr<ASTnode> _returnType,
                               std::unique_ptr<ASTnode> _block, Token* _token)
                               : ASTnode(_token), name(_name), params(std::move(_params)),
                                 returnType(std::move(_returnType)), block(std::move(_block))
{
    
}

std::unique_ptr<ASTvalue> FunDeclaration::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<FunDeclaration*>(this));
}

VarList::VarList(std::vector<std::unique_ptr<ASTnode>> _varStatements, Token* _token)
                 : ASTnode(_token), varStatments(std::move(_varStatements)) 
{
    
}

std::unique_ptr<ASTvalue> VarList::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<VarList*>(this));
}

StructDeclaration::StructDeclaration(std::string _name, std::unique_ptr<ASTnode> _varList,
                                     Token* _token)
                                     : ASTnode(_token), name(_name), varList(std::move(_varList)) 
{
    
}

std::unique_ptr<ASTvalue> StructDeclaration::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<StructDeclaration*>(this));
}

DeclarationList::DeclarationList(std::vector<std::unique_ptr<ASTnode>> _declarations, Token* _token)
                                 : ASTnode(_token), declarations(std::move(_declarations)) 
{
    
}

std::unique_ptr<ASTvalue> DeclarationList::execute(AstnodeOperator* operation)
{
    return operation->execute(dynamic_cast<DeclarationList*>(this));
}

