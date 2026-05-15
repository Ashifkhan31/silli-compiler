#include "compiler.h"

Compiler::Compiler(ASTnode* _root, std::string& _code) : root(_root), code(_code)
{    
    code.reserve(1000);

    std::cout<<"NOTE: Only supports x64 Linux machines.\n";

    if (sizeof(int) == 4 && sizeof(double) == 8) return;

    std::cout<<"Int and double are not of expected sizes.\n";
    hadError = true;
}

bool Compiler::compile()
{
    if (hadError) return false;

    code.append("    .globl main\n");

    code.append(".LC0:\n");
    code.append("    .string \"%f\\n\" \n");

    code.append("main:\n");
    code.append("    pushq %rbp\n");
    code.append("    movq %rsp, %rbp\n");

    execute(root);

    code.append("    movq (%rsp), %rax\n");
    code.append("    movq %rax, %xmm0\n");
    code.append("    addq $8, %rsp\n");
    code.append("    leaq .LC0(%rip), %rax\n");
    code.append("    movq %rax, %rdi\n");
    code.append("    movl $1, %eax\n");
    code.append("    call printf\n");


    code.append("    movl $0, %eax\n");
    code.append("    leave\n");
    code.append("    ret\n");

    code.append(constants);

    return !hadError;
}

CompilerValue* Compiler::execute(ASTnode* node)
{
    return dynamic_cast<CompilerValue*>(AstnodeOperator::execute(node));
}

CompilerValue* Compiler::execute(TermNode* node)
{
    node->left->execute(this);
    node->right->execute(this);

    code.append("    movsd 8(%rsp), %xmm0\n");

    if (node->token->type == TokenType::PLUS)
    {
        code.append("    addsd (%rsp), %xmm0\n");
    }
    else
    {
        code.append("    subsd (%rsp), %xmm0\n");
    }

    code.append("    addq $8, %rsp\n");
    code.append("    movsd %xmm0, (%rsp)\n");
    return nullptr;
}

CompilerValue* Compiler::execute(FactorNode* node)
{
    node->left->execute(this);
    node->right->execute(this);

    code.append("    movsd 8(%rsp), %xmm0\n");

    if (node->token->type == TokenType::STAR)
    {
        code.append("    mulsd (%rsp), %xmm0\n");
    }
    else
    {
        code.append("    divsd (%rsp), %xmm0\n");
    }

    code.append("    addq $8, %rsp\n");
    code.append("    movsd %xmm0, (%rsp)\n");
    return nullptr;
}

CompilerValue* Compiler::execute(UnaryNode* node)
{
    node->node->execute(this);
    
    code.append("    pxor %xmm0, %xmm0\n");
    code.append("    subsd (%rsp), %xmm0\n");
    code.append("    movsd %xmm0, (%rsp)\n");
    return nullptr;
}

CompilerValue* Compiler::execute(PrimaryNode* node)
{
    node->node->execute(this);
    return nullptr;
}

CompilerValue* Compiler::execute(NumberNode* node)
{
    noOfConstants += 1;
    
    code.append("    subq $8, %rsp\n");
    code.append("    movsd .LC" + std::to_string(noOfConstants) + "(%rip), %xmm0\n");
    code.append("    movsd %xmm0, (%rsp)\n");

    int a = *reinterpret_cast<int*>((&node->num));
    int b = *(reinterpret_cast<int*>((&node->num)) + 1);

    constants.append(".LC" + std::to_string(noOfConstants) + ":\n");
    constants.append("    .long " + std::to_string(a) + "\n");
    constants.append("    .long " + std::to_string(b) + "\n");
    return nullptr;
}
