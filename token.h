#ifndef token_h
#define token_h

#include <utility>

enum class TokenType
{
    INTEGER, DOUBLE, CHARACTER, STRING, TRUE, FALSE,

    INT, REAL, CHAR, BOOL, DOLLAR,

    IDENTIFIER, STRUCT, FUN, SET, IF, ELIF, ELSE, WHILE, FOR, CAST, AND, OR,
    
    EQUAL_EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL,
    PLUS, MINUS, STAR, SLASH, PERCENT, NOT,

    EQUAL, COLON, SEMI_COLON, LEFT_BRACKET, RIGHT_BRACKET, LEFT_BRACE,
    RIGHT_BRACE, LEFT_PAREN, RIGHT_PAREN, COMMA, UP, DOT,

    END      
};

struct Token
{
    TokenType type;    
    const char* start;
    std::size_t length;
    std::size_t line;
};

#endif
