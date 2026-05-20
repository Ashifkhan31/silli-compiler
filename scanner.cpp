#include "common.h"
#include "scanner.h"

#include <cstring>

Scanner::Scanner(const std::string& _source, std::vector<Token>& _tokens)
                : source(_source), tokens(_tokens)  
{
    tokens.reserve(200);
}

void Scanner::consume(TokenType type)
{
    tokens.push_back(Token{type, &source[start], current - start, line});
    start = current;
}

void Scanner::skip()
{
    start = current;
}

void Scanner::advance()
{
    if (current < source.size()) current++;
}

bool Scanner::isAtEnd()
{
    if(current >= source.size()) return true;
    return false;
}

char Scanner::peek()
{
    if (current >= source.size()) return '\0';
    return source[current];
}

char Scanner::peekNext()
{
    if (current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

bool isDigit(char c)
{
    if ('0' <= c && '9' >= c) return true;
    return false;
}

void Scanner::character()
{    
    if (peek() != '\'' && peekNext() == '\'')
    {
        advance();
        advance();
        consume(TokenType::CHARACTER);
        return;
    }

    std::cout<<"Unexpected character "<<"'"<<"'"<<"'"<<" in line "<<line<<".\n";
    skip();
}

void Scanner::string()
{
    while(peek() != '"' && !isAtEnd())
    {
        advance();
    }

    if (peek() == '"')
    {
        advance();
        consume(TokenType::STRING);
        return;
    }

    std::cout<<"Unterminated string at line "<<line<<".\n";
    skip();
}

void Scanner::number()
{
    while(isDigit(peek()))
    {
        advance();
    }

    if (peek() != '.' || !isDigit(peekNext()))
    {
        consume(TokenType::INTEGER);
        return;
    }

    advance();
    while(isDigit(peek()))
    {
        advance();
    }

    consume(TokenType::DOUBLE);
}

bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::checkKeyword(const char* str, int index, int length)
{
    return (current - (start + index) == length) &&
           std::memcmp(&source[start + index], str, length * sizeof(char)) == 0;
}

bool Scanner::isKeyword(TokenType& type)
{
    
    switch (source[start])
    {
        case 'a':
            if (checkKeyword("nd", 1, 2))
            {
                type = TokenType::AND;
                return true;
            }
        case 'b':
            if (checkKeyword("ool", 1, 3))
            {
                type = TokenType::BOOL;
                return true;
            }
        case 'r':
            if (checkKeyword("eal", 1, 3))
            {
                type = TokenType::REAL;
                return true;
            }
        case 't':
            if (checkKeyword("rue", 1, 3))
            {
                type = TokenType::TRUE;
                return true;
            }
        case 'w':
            if (checkKeyword("hile", 1, 4))
            {
                type = TokenType::WHILE;
                return true;
            }
        case 'o':
            if (checkKeyword("r", 1, 1))
            {
                type = TokenType::OR;
                return true;
            }            
        case 'c':
            if (checkKeyword("har", 1, 3))
            {
                type = TokenType::CHAR;
                return true;
            }
            else if (checkKeyword("ast", 1, 3))
            {
                type = TokenType::CAST;
                return true;
            }
            return false;
        case 'e':
            if (checkKeyword("lse", 1, 3))
            {
                type = TokenType::ELSE;
                return true;
            }
            else if (checkKeyword("lif", 1, 3))
            {
                type = TokenType::ELIF;
                return true;
            }
            return false;
        case 'i':
            if (checkKeyword("f", 1, 1))
            {
                type = TokenType::IF;
                return true;
            }
            else if (checkKeyword("nt", 1, 2))
            {
                type = TokenType::INT;
                return true;
            }
            return false;
        case 'f':
            if (checkKeyword("un", 1, 2))
            {
                type = TokenType::FUN;
                return true;
            }
            else if (checkKeyword("or", 1, 2))
            {
                type = TokenType::FOR;
                return true;
            }
            return false;
        case 's':
            if (checkKeyword("truct", 1, 5))
            {
                type = TokenType::STRUCT;
                return true;
            }
            else if (checkKeyword("et", 1, 2))
            {
                type = TokenType::SET;
                return true;
            }
            return false;
    }

    return false;
}

void Scanner::identifier()
{
    while (isAlpha(peek()) || isDigit(peek()))
    {
        advance();
    }

    TokenType type;
    if (isKeyword(type))
    {
        consume(type);
    }
    else
    {
        consume(TokenType::IDENTIFIER);
    }
}

void Scanner::scanTokens()
{
    char c = peek();
    advance();

    if (c == '\'') return character();

    if (c == '"') return string();

    if (isDigit(c)) return number();

    if (isAlpha(c)) return identifier();
    
    switch (c)
    {
        case '+':
            consume(TokenType::PLUS);
            return;
        case '-':
            consume(TokenType::MINUS);
            return;
        case '*':
            consume(TokenType::STAR);
            return;
        case '/':
            consume(TokenType::SLASH);
            return;
        case '%':
            consume(TokenType::PERCENT);
            return;
        case ':':
            consume(TokenType::COLON);
            return;
        case ';':
            consume(TokenType::SEMI_COLON);
            return;
        case '{':
            consume(TokenType::LEFT_BRACE);
            return;
        case '}':
            consume(TokenType::RIGHT_BRACE);
            return;
        case '(':
            consume(TokenType::LEFT_PAREN);
            return;
        case ')':
            consume(TokenType::RIGHT_PAREN);
            return;
        case ',':
            consume(TokenType::COMMA);
            return;
        case '^':
            consume(TokenType::UP);
            return;
        case '.':
            consume(TokenType::DOT);
            return;
        case '$':
            consume(TokenType::DOLLAR);
            return;
        case '[':
            consume(TokenType::LEFT_BRACKET);
            return;
        case ']':
            consume(TokenType::RIGHT_BRACKET);
            return;
        case '=':
            if(peek() == '=')
            {
                advance();
                consume(TokenType::EQUAL_EQUAL);
                return;
            }
            consume(TokenType::EQUAL);
            return;
        case '!':
            if(peek() == '=')
            {
                advance();
                consume(TokenType::NOT_EQUAL);
                return;
            }
            consume(TokenType::NOT);
            return;
        case '<':
            if(peek() == '=')
            {
                advance();
                consume(TokenType::LESS_EQUAL);
                return;
            }
            consume(TokenType::LESS);
            return;
        case '>':
            if(peek() == '=')
            {
                advance();
                consume(TokenType::GREAT_EQUAL);
                return;
            }
            consume(TokenType::GREAT);
            return;
        case '\n':
            line++;
            skip();
            return;
        case ' ':
        case '\t':
        case '\r':
            skip();
            return;
    }

    hadError = true;
    std::cout<<"Unexpected character "<<"'"<<c<<"'"<<" in line "<<line<<".\n";
    skip();
}


void Scanner::printTokens()
{
    for (Token token : tokens)
    {
        char* str = new char[token.length + 1];
        std::memcpy(str, token.start, sizeof(char) * token.length);
        str[token.length] = '\0';
        
        std::cout<<"Token: "<<str<<" Type: "<<static_cast<int>(token.type)
        <<" Line: "<<token.line<<".\n";
    }
}

void Scanner::scan()
{
    while(!isAtEnd())
    {
        scanTokens();
    }
    tokens.push_back(Token{TokenType::END, "", 0, line});

}
