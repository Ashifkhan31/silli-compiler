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

    CompilerValue* execute(ASTnode* node) override;
    CompilerValue* execute(TermNode* node) override;
    CompilerValue* execute(FactorNode* node) override;
    CompilerValue* execute(UnaryNode* node) override;
    CompilerValue* execute(PrimaryNode* node) override;
    CompilerValue* execute(NumberNode* node) override;    

    public:
    Compiler(ASTnode* _root, std::string& _code);
    bool compile();
};

#endif 
