#ifndef scanner_h
#define scanner_h

#include "common.h"
#include "token.h"

class Scanner
{
    int index = 0;
    const std::string& source;
    std::vector<Token>& tokens;

    char peek();
    void consume();
    bool isAtEnd();
    Token makeToken(TokenType type, int start, int length);
    Token makeEndToken();
    void scanTokens();
    void number();

    public: 

    bool hadError = false;
    Scanner(const std::string& _source, std::vector<Token>& _tokens);
    void scan();
    void printTokens();
};

#endif
