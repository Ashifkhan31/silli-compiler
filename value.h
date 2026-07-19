#ifndef value_h
#define value_h

#include "ASTnode.h"

enum class ValueType
{
    INTEGER,
    REAL,
    CHAR,
    BOOL,
    ARRAY,
    REFERENCE,
};

class Value : public ASTvalue
{
    public:
    ValueType type;

    protected:
    Value(ValueType _type) : ASTvalue(), type(_type) {}
};

class IntegerValue : public Value
{
    public:
    int number;
    IntegerValue(int _number) : Value(ValueType::INTEGER), number(_number) {}
};

class RealValue : public Value
{
    public:
    double number;
    RealValue(double _number) : Value(ValueType::REAL), number(_number) {}
};

class BoolValue : public Value
{
    public:
    bool value;;
    BoolValue(bool _value) : Value(ValueType::BOOL), value(_value) {}
};

class CharacterValue : public Value
{
    public:
    char character;
    CharacterValue(char _character) : Value(ValueType::CHAR), character(_character) {}
};

class ArrayValue : public Value
{
    public:
    std::vector<std::unique_ptr<Value>> elements;

    ArrayValue() : Value(ValueType::ARRAY)
    {
        elements.reserve(10);
    }
};

class ReferenceValue : public Value
{
    public:
    Value* val;
    ReferenceValue(Value* _val) : Value(ValueType::REFERENCE), val(_val) {}
};

#endif
