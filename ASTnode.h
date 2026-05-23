#ifndef ASTnode_h
#define ASTnode_h

#include <memory>
#include "token.h"
#include "common.h"

class AstnodeOperator;

enum class ASTnodeType
{
    INTEGER,
    DOUBLE,
    CHARACTER,
    STRING,
    BOOLEAN,
    UNARY,
};

enum class ExprType
{
    INTEGER,
    DOUBLE,
    CHARACTER,
    STRING,
    BOOLEAN,
    VOID,
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
    ExprType exprType;
    
    virtual ASTvalue* execute(AstnodeOperator* operation) = 0;

    protected:
    ASTnode(ASTnodeType _type) : type(_type) {}
};

class Integer;
class Double;
class Character;
class String;
class Boolean;
class Unary;

class AstnodeOperator
{
    protected:
    AstnodeOperator(){}

    public:
    virtual ASTvalue* execute(ASTnode* node);
    virtual ASTvalue* execute(Integer* node) = 0;
    virtual ASTvalue* execute(Double* node) = 0;
    virtual ASTvalue* execute(Character* node) = 0;
    virtual ASTvalue* execute(String* node) = 0;
    virtual ASTvalue* execute(Boolean* node) = 0;
    virtual ASTvalue* execute(Unary* node) = 0;
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

class String : public ASTnode
{
    public:
    std::string str;
    Token* token;

    String(std::string _str, Token* _token);
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

#endif
