#ifndef common_h
#define common_h

#include <vector>
#include <iostream>
#include <string>
#include "unordered_map"
#include "token.h"

class Error
{
    public:
    std::string message;
    
    Error(std::string _message) : message(_message) {}

    Error(Token* _token, std::string _message)
    {
        message = "[line " + std::to_string(_token->line) + "]: " + _message;
    }
    
    void print()
    {
        std::cout<<message<<"\n";
    }
};

#endif
