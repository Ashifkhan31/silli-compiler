#ifndef ASTnode_h
#define ASTnode_h

#include <memory>
#include "token.h"
#include "common.h"
#include "typeclass.h"

class AstnodeOperator;

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
    std::unique_ptr<TypeClass> typeClass;
    Token* token; 
        
    virtual std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation) = 0;

    protected:
    ASTnode(Token* _token) {}
};

class Program;
class Int;
class Real;
class Char;
class Bool;
class TypeName;
class IsArray;
class IsReference;
class FnType;
class Integer;
class Double;
class Character;
class Boolean;
class Array;
class String;
class ArrayAllocation;
class StructValue;
class Identifier;
class ArrayElementAccess;
class StructElementAccess;
class Call;
class Cast;
class Group;
class UnaryOperation;
class BinaryOperation;
class VarDecl;
class AssignmentStatement;
class ExpressionStatement;
class StatementList;
class Block;
class WhileStatement;
class ForStatement;
class IfStatement;
class ElifStatement;
class ElseStatement;
class BreakStatement;
class ContinueStatement;
class Parameter;
class FunDeclaration;
class VarList;
class StructDeclaration;
class ReferDeclaration;
class DeclarationList;

class AstnodeOperator
{
    protected:
    AstnodeOperator(){}

    public:

    virtual std::unique_ptr<ASTvalue> execute(Program* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Int* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Real* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Char* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Bool* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(TypeName* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(IsArray* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(IsReference* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(FnType* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Integer* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Double* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Character* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Boolean* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Array* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(String* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(ArrayAllocation* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(StructValue* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Identifier* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(ArrayElementAccess* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(StructElementAccess* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Call* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Cast* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Group* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(UnaryOperation* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(BinaryOperation* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(VarDecl* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(AssignmentStatement* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(ExpressionStatement* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(StatementList* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Block* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(WhileStatement* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(ForStatement* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(IfStatement* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(ElifStatement* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(ElseStatement* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(BreakStatement* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(ContinueStatement* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(Parameter* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(FunDeclaration* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(VarList* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(StructDeclaration* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(ReferDeclaration* node) = 0;
    virtual std::unique_ptr<ASTvalue> execute(DeclarationList* node) = 0;

};

class Program : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> node;

    Program(std::unique_ptr<ASTnode> _node, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Int : public ASTnode
{
    public:

    Int(Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Real : public ASTnode
{
    public:

    Real(Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Char : public ASTnode
{
    public:

    Char(Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Bool : public ASTnode
{
    public:

    Bool(Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class TypeName : public ASTnode
{
    public:
    std::string name;
    
    TypeName(std::string _name, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class FnType : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> returnType;
    std::vector<std::unique_ptr<ASTnode>> paramTypes;
    
    FnType(std::unique_ptr<ASTnode> _returnType,
           std::vector<std::unique_ptr<ASTnode>> _paramTypes,
           Token* _token);

    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class IsArray : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> type;
    
    IsArray(std::unique_ptr<ASTnode> _type, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class IsReference : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> type;
    
    IsReference(std::unique_ptr<ASTnode> _type, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Integer : public ASTnode
{
    public:
    int number;

    Integer(int _number, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Double : public ASTnode
{
    public:
    double number;

    Double(double _number, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Character : public ASTnode
{
    public:
    char c;

    Character(char _c, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Boolean : public ASTnode
{
    public:
    bool truthValue;

    Boolean(bool _truthValue, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Array : public ASTnode
{
    public:
    std::vector<std::unique_ptr<ASTnode>> elements;

    Array(std::vector<std::unique_ptr<ASTnode>> _elements, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class String : public ASTnode
{
    public:
    std::string str;

    String(std::string _str, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class ArrayAllocation : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> type;
    std::unique_ptr<ASTnode> size;

    ArrayAllocation(std::unique_ptr<ASTnode> _type, std::unique_ptr<ASTnode> _size, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class StructValue : public ASTnode
{
    public:
    std::string structName;
    std::vector<std::unique_ptr<ASTnode>> elements;

    StructValue(std::string _structName,
                std::vector<std::unique_ptr<ASTnode>> _elements,
                Token* _token);
    
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Identifier : public ASTnode
{
    public:
    std::string name;

    Identifier(std::string _name, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class ArrayElementAccess : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> array;
    std::unique_ptr<ASTnode> index;

    ArrayElementAccess(std::unique_ptr<ASTnode> _array, std::unique_ptr<ASTnode> index,
                       Token* _token);
    
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class StructElementAccess: public ASTnode
{
    public:
    std::unique_ptr<ASTnode> base;
    std::string name;

    StructElementAccess(std::unique_ptr<ASTnode> _base, std::string _name, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Call : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> base;
    std::vector<std::unique_ptr<ASTnode>> args;
    
    Call(std::unique_ptr<ASTnode> _base,
         std::vector<std::unique_ptr<ASTnode>> _args,
         Token* _token);

    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Cast : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> type;
    std::unique_ptr<ASTnode> expr;
    
    Cast(std::unique_ptr<ASTnode> _type, std::unique_ptr<ASTnode> _expr, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Group : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> expr;
    
    Group(std::unique_ptr<ASTnode> _expr, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class UnaryOperation : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> value;
    
    UnaryOperation(std::unique_ptr<ASTnode> _value, Token* _token);

    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class BinaryOperation : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> left;
    std::unique_ptr<ASTnode> right;
    
    BinaryOperation(std::unique_ptr<ASTnode> _left, std::unique_ptr<ASTnode> _right, Token* _token);

    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class VarDecl : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> type;
    std::string name;
    std::unique_ptr<ASTnode> expr;
    
    VarDecl(std::unique_ptr<ASTnode> _type, std::string _name,
            std::unique_ptr<ASTnode> _expr, Token* _token);
    
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class AssignmentStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> variable;
    std::unique_ptr<ASTnode> expr;
    
    AssignmentStatement(std::unique_ptr<ASTnode> _variable,
                        std::unique_ptr<ASTnode> _expr, Token* _token);
    
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class ExpressionStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> expr;
    
    ExpressionStatement(std::unique_ptr<ASTnode> _expr, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class StatementList : public ASTnode
{
    public:
    std::vector<std::unique_ptr<ASTnode>> statements;
    
    StatementList(std::vector<std::unique_ptr<ASTnode>> _statements, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Block : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> list;
    
    Block(std::unique_ptr<ASTnode> _list, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class WhileStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> condition;
    std::unique_ptr<ASTnode> body;
    
    WhileStatement(std::unique_ptr<ASTnode> _condition, std::unique_ptr<ASTnode> _body,
                   Token* _token);
    
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class ForStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> init;
    std::unique_ptr<ASTnode> condition;
    std::unique_ptr<ASTnode> update;
    std::unique_ptr<ASTnode> body;
    
    ForStatement(std::unique_ptr<ASTnode> _init, std::unique_ptr<ASTnode> _condition,
                 std::unique_ptr<ASTnode> _update, std::unique_ptr<ASTnode> _body, Token* _token);
    
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class IfStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> condition;
    std::unique_ptr<ASTnode> body;
    std::unique_ptr<ASTnode> tail;
    
    IfStatement(std::unique_ptr<ASTnode> _condition, std::unique_ptr<ASTnode> _body,
                std::unique_ptr<ASTnode> _tail, Token* _token);
    
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class ElifStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> condition;
    std::unique_ptr<ASTnode> body;
    std::unique_ptr<ASTnode> tail;
    
    ElifStatement(std::unique_ptr<ASTnode> _condition, std::unique_ptr<ASTnode> _body,
                std::unique_ptr<ASTnode> _tail, Token* _token);
    
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class ElseStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> body;
    
    ElseStatement(std::unique_ptr<ASTnode> _body, Token* _token);
    
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class BreakStatement : public ASTnode
{
    public:
        
    BreakStatement(Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class ContinueStatement : public ASTnode
{
    public:
        
    ContinueStatement(Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class Parameter : public ASTnode
{    
    public:
    std::unique_ptr<ASTnode> type;
    std::string name;
    
    Parameter(std::unique_ptr<ASTnode> _type, std::string _name, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class FunDeclaration : public ASTnode
{
    public:
    std::string name;
    std::vector<std::unique_ptr<ASTnode>> params;
    std::unique_ptr<ASTnode> returnType;
    std::unique_ptr<ASTnode> block;
    
    FunDeclaration(std::string _name,
                   std::vector<std::unique_ptr<ASTnode>> _params,
                   std::unique_ptr<ASTnode> _returnType,
                   std::unique_ptr<ASTnode> _block,
                   Token* _token);

    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class VarList : public ASTnode
{
    public:
    std::vector<std::unique_ptr<ASTnode>> varStatments;  

    VarList(std::vector<std::unique_ptr<ASTnode>> _varStatements, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class StructDeclaration : public ASTnode
{
    public:
    std::string name;
    std::unique_ptr<ASTnode> varList;
    
    StructDeclaration(std::string _name, std::unique_ptr<ASTnode> _varlist, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class ReferDeclaration : public ASTnode
{
    public:
    std::string path;
    
    ReferDeclaration(std::string _path, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

class DeclarationList : public ASTnode
{
    public:
    std::vector<std::unique_ptr<ASTnode>> declarations;
    
    DeclarationList(std::vector<std::unique_ptr<ASTnode>> _declarations, Token* _token);
    std::unique_ptr<ASTvalue> execute(AstnodeOperator* operation);
};

#endif
