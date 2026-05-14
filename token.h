#ifndef token_h
#define token_h

enum class TokenType
{
    NUMBER,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    LEFT_PAREN,
    RIGHT_PAREN,
    END,
};

struct Token
{
    TokenType type;    
    const char* start;
    int length;
};

#endif
