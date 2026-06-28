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
        
    virtual ASTvalue* execute(AstnodeOperator* operation) = 0;

    protected:
    ASTnode() {}
};

class Name;
class Integer;
class Double;
class Character;
class Boolean;
class Unary;
class Factor;
class Term;
class Relational;
class Equivalent;
class Logical;
class VarDecl;
class SetVar;
class PrintStmt;
class BlockStatement;
class IfStatement;
class ElifStatement;
class ElseStatement;
class WhileStatement;
class ForStatement;
class StatementList;
class Program;

class AstnodeOperator
{
    protected:
    AstnodeOperator(){}

    public:
    virtual ASTvalue* execute(Program* node) = 0;
    virtual ASTvalue* execute(Integer* node) = 0;
    virtual ASTvalue* execute(Double* node) = 0;
    virtual ASTvalue* execute(Character* node) = 0;
    virtual ASTvalue* execute(Boolean* node) = 0;
    virtual ASTvalue* execute(Unary* node) = 0;
    virtual ASTvalue* execute(Factor* node) = 0;
    virtual ASTvalue* execute(Term* node) = 0;
    virtual ASTvalue* execute(Relational* node) = 0;
    virtual ASTvalue* execute(Equivalent* node) = 0;
    virtual ASTvalue* execute(Logical* node) = 0;
    virtual ASTvalue* execute(VarDecl* node) = 0;
    virtual ASTvalue* execute(SetVar* node) = 0;
    virtual ASTvalue* execute(PrintStmt* node) = 0;
    virtual ASTvalue* execute(StatementList* node) = 0;
    virtual ASTvalue* execute(IfStatement* node) = 0;
    virtual ASTvalue* execute(ElseStatement* node) = 0;
    virtual ASTvalue* execute(ElifStatement* node) = 0;
    virtual ASTvalue* execute(WhileStatement* node) = 0;
    virtual ASTvalue* execute(ForStatement* node) = 0;
    virtual ASTvalue* execute(BlockStatement* node) = 0;
    virtual ASTvalue* execute(Name* node) = 0;
};

class Integer : public ASTnode
{
    public:
    int number;
    Token* token;

    Integer(int _number, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Double : public ASTnode
{
    public:
    double number;
    Token* token;

    Double(double _number, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Character : public ASTnode
{
    public:
    char c;
    Token* token;

    Character(char _c, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Boolean : public ASTnode
{
    public:
    bool value;
    Token* token;

    Boolean(bool _value, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Unary : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> child;
    Token* token;

    Unary(ASTnode* _child, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Factor : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> left;
    std::unique_ptr<ASTnode> right;
    Token* token;

    Factor(ASTnode* _left, ASTnode* _right, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Term : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> left;
    std::unique_ptr<ASTnode> right;
    Token* token;

    Term(ASTnode* _left, ASTnode* _right, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Relational : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> left;
    std::unique_ptr<ASTnode> right;
    Token* token;

    Relational(ASTnode* _left, ASTnode* _right, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Equivalent : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> left;
    std::unique_ptr<ASTnode> right;
    Token* token;

    Equivalent(ASTnode* _left, ASTnode* _right, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Logical : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> left;
    std::unique_ptr<ASTnode> right;
    Token* token;

    Logical(ASTnode* _left, ASTnode* _right, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Program : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> node;

    Program(ASTnode* _node);
    ASTvalue* execute(AstnodeOperator* operation);
};

class VarDecl : public ASTnode
{
    public:
    Token* dataType;
    std::string name;
    Token* token;
    std::unique_ptr<ASTnode> expr;

    VarDecl(Token* _dataType, std::string _name, ASTnode* _expr, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class SetVar : public ASTnode
{
    public:
    std::string name;
    std::unique_ptr<ASTnode> expr;
    Token* token;

    SetVar(std::string _name, ASTnode* _expr, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class StatementList : public ASTnode
{
    public:
    std::vector<std::unique_ptr<ASTnode>> list;

    StatementList();
    ASTvalue* execute(AstnodeOperator* operation);
};

class BlockStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> stmtList;

    BlockStatement(ASTnode* _stmtList);
    ASTvalue* execute(AstnodeOperator* operation);
};

class Name : public ASTnode
{
    public:
    std::string name;
    Token* token;
    
    Name(std::string _name, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class PrintStmt : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> expr;

    PrintStmt(ASTnode* _expr);
    ASTvalue* execute(AstnodeOperator* operation);
};

class IfStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> condition;
    std::unique_ptr<ASTnode> block;
    std::unique_ptr<ASTnode> tail;
    Token* token;

    IfStatement(ASTnode* _condition, ASTnode* _block, ASTnode* _tail, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class ElifStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> condition;
    std::unique_ptr<ASTnode> block;
    std::unique_ptr<ASTnode> tail;
    Token* token;

    ElifStatement(ASTnode* _condition, ASTnode* _block, ASTnode* _tail, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class ElseStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> block;
    Token* token;

    ElseStatement(ASTnode* _block, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class WhileStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> condition;
    std::unique_ptr<ASTnode> block;
    Token* token;

    WhileStatement(ASTnode* _condition, ASTnode* _block, Token* _token);
    ASTvalue* execute(AstnodeOperator* operation);
};

class ForStatement : public ASTnode
{
    public:
    std::unique_ptr<ASTnode> initializer;
    std::unique_ptr<ASTnode> condition;
    std::unique_ptr<ASTnode> updator;
    std::unique_ptr<ASTnode> block;
    Token* token;

    ForStatement(ASTnode* _initialzer, ASTnode* _condition ,ASTnode* _updator,
                 ASTnode* _block, Token* _token);
    
    ASTvalue* execute(AstnodeOperator* operation);
};

#endif
