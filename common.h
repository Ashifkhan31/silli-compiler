#ifndef common_h
#define common_h

#include <vector>
#include <iostream>
#include <string>

class Error
{
    public:
    std::string message;
    
    Error(std::string _message) : message(_message) {}
    void print()
    {
        std::cout<<message<<"\n";
    }
};

#endif
