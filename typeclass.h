#ifndef typeclass_h
#define typeclass_h

#include "common.h"

enum class Type
{
    INTEGER,
    DOUBLE,
    CHARACTER,
    BOOLEAN,
    ARRAY,
    REFERENCE,
    VOID,
};

class TypeClass
{
    public:
    Type baseType;
    virtual ~TypeClass() {}

    protected:
    TypeClass(Type _baseType) : baseType(_baseType) {}
};

class IntegerType : public TypeClass
{
    public:
    IntegerType() : TypeClass(Type::INTEGER) {}
};

class DoubleType : public TypeClass
{
    public:
    DoubleType() : TypeClass(Type::DOUBLE) {}
};

class CharacterType : public TypeClass
{
    public:
    CharacterType() : TypeClass(Type::CHARACTER) {}
};

class BooleanType : public TypeClass
{
    public:
    BooleanType() : TypeClass(Type::BOOLEAN) {}
};

class ArrayType : public TypeClass
{
    public:
    std::unique_ptr<TypeClass> elementType;

    ArrayType(std::unique_ptr<TypeClass> _elementType)
              : TypeClass(Type::ARRAY), elementType(std::move(_elementType)) {}
};

class ReferenceType : public TypeClass
{
    public:
    std::unique_ptr<TypeClass> simpleType;

    ReferenceType(std::unique_ptr<TypeClass> _simpleType)
                  : TypeClass(Type::REFERENCE), simpleType(std::move(_simpleType)) {}  
};

class VoidType : public TypeClass
{
    public:
    VoidType() : TypeClass(Type::VOID) {}
};

#endif
