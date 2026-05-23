#include "compiler.h"

Compiler::Compiler(ASTnode* _root, std::string& _code)
                   : root(_root), code(_code)
{
    code.reserve(1000);
    constants.reserve(1000);
    
    intSize = sizeof(int);
    doubleSize = sizeof(double);    
}

bool Compiler::compile()
{
    return true;
}

CompilerValue* Compiler::execute(ASTnode* node)
{
    return dynamic_cast<CompilerValue*>(AstnodeOperator::execute(node));
}

