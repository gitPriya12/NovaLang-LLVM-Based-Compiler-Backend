#pragma once

#include <string>

enum class TokenType
{
    // Keywords
    LET,
    FN,
    IF,
    ELSE,
    WHILE,
    RETURN,

    // Literals
    IDENTIFIER,
    NUMBER,

    // Operators
    PLUS,
    MINUS,
    STAR,
    SLASH,

    ASSIGN,

    EQUAL_EQUAL,

    LESS,
    LESS_EQUAL,

    GREATER,
    GREATER_EQUAL,

    NOT_EQUAL,

    // Symbols
    LPAREN,
    RPAREN,

    LBRACE,
    RBRACE,

    COMMA,
    SEMICOLON,

    END_OF_FILE,

    INVALID

};

struct Token
{
    TokenType type;

    std::string lexeme;

    int line;

    Token(
        TokenType t,
        const std::string& l,
        int ln)
        :
        type(t),
        lexeme(l),
        line(ln)
    {
    }
};