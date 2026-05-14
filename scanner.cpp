#include "common.h"
#include "scanner.h"

#include <cstring>

Scanner::Scanner(const std::string& _source, std::vector<Token>& _tokens)
                : source(_source), tokens(_tokens)  
{
    tokens.reserve(150);
}

void Scanner::consume()
{
    if(index >= source.size()) return;

    index++;
    return;
}

bool Scanner::isAtEnd()
{
    if(index >= source.size()) return true;
    return false;
}

char Scanner::peek()
{
    if (index >= source.size()) return '\0';
    return source[index];
}

bool isDigit(char c)
{
    if ('0' <= c && '9' >= c) return true;
    return false;
}

void Scanner::number()
{
    int start = index;
    while(isDigit(peek()))
    {
        consume();
    }

    tokens.push_back(makeToken(TokenType::NUMBER, start, index - start));
}

Token Scanner::makeToken(TokenType type, int start, int length)
{
    return Token{type, &source[start], length};
}

void Scanner::scanTokens()
{
    if(isDigit(peek())) return number();

    switch (peek())
    {
        case '+':
            tokens.push_back(makeToken(TokenType::PLUS, index, 1));
            consume();
            return;
        case '-':
            tokens.push_back(makeToken(TokenType::MINUS, index, 1));
            consume();
            return;;
        case '*':
            tokens.push_back(makeToken(TokenType::STAR, index, 1));
            consume();
            return;
        case '/':
            tokens.push_back(makeToken(TokenType::SLASH, index, 1));
            consume();
            return;
        case '(':
            tokens.push_back(makeToken(TokenType::LEFT_PAREN, index, 1));
            consume();
            return;
        case ')':
            tokens.push_back(makeToken(TokenType::RIGHT_PAREN, index, 1));
            consume();
            return;
        case ' ':
        case '\n':
        case '\t':
        case '\r':
            consume();
            return;
    }

    hadError = true;
    std::cout<<"Unexpected character '"<<peek()<<"'.\n";
    consume();
}

Token Scanner::makeEndToken()
{
    return Token{TokenType::END, "", 0};
}

void Scanner::printTokens()
{
    for (Token token : tokens)
    {
        char* str = new char[token.length + 1];
        std::memcpy(str, token.start, sizeof(char) * token.length);
        str[token.length] = '\0';
        
        std::cout<<"Token: "<<str<<" Type: "<<static_cast<int>(token.type)<<".\n";
    }
}

void Scanner::scan()
{
    while(!isAtEnd())
    {
        scanTokens();
    }
    tokens.push_back(makeEndToken());
}
