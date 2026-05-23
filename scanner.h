#ifndef scanner_h
#define scanner_h

#include "common.h"
#include "token.h"

class Scanner
{
    std::size_t start = 0;
    std::size_t current = 0;
    std::size_t line = 1;
    const std::string& source;
    std::vector<Token>& tokens;

    char peek();
    char peekNext();
    void advance();
    void consume(TokenType type);
    void skip();
    bool isAtEnd();
    void character();
    void string();
    void number();
    bool checkKeyword(const char* str, int index, int length);
    bool isKeyword(TokenType& type);
    void identifier();
    void scanTokens();

    public: 

    bool hadError = false;
    Scanner(const std::string& _source, std::vector<Token>& _tokens);
    void scan();
    void printTokens();
};

#endif
