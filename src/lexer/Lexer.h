#pragma once

#include <vector>
#include <unordered_map>

#include "Token.h"

class Lexer
{
private:

    std::string source;

    size_t current;

    int line;

    std::unordered_map<
        std::string,
        TokenType> keywords;

public:

    Lexer(const std::string& src);

    std::vector<Token> tokenize();

private:

    bool isAtEnd() const;

    char peek() const;

    char advance();

    void skipWhitespace();

    Token scanIdentifier();

    Token scanNumber();

    Token makeToken(
        TokenType type,
        const std::string& lexeme);
};