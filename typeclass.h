#ifndef typeclass_h
#define typeclass_h

enum class Type
{
    INTEGER,
    DOUBLE,
    CHARACTER,
    BOOLEAN,
    VOID,
};

class TypeClass
{
    public:
    Type baseType;

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

class VoidType : public TypeClass
{
    public:
    VoidType() : TypeClass(Type::VOID) {}
};

#endif
