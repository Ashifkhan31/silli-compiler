#ifndef compiler_h
#define compiler_h

#include "ASTnode.h"
#include <string>

class CompilerValue : public ASTvalue
{
    
};

class Compiler : public AstnodeOperator
{
    bool hadError = false;
    ASTnode* root;
    std::string& code;
    std::string constants;
    int noOfConstants = 0;
    int intSize;
    int doubleSize;

    CompilerValue* execute(ASTnode* node) override;

    public:
    Compiler(ASTnode* _root, std::string& _code);
    bool compile();
};

#endif 
