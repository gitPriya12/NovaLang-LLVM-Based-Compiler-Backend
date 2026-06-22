#include "Lexer.h"

#include <cctype>

Lexer::Lexer(const std::string& src)
    : source(src),
    current(0),
    line(1)
{
    keywords["let"] = TokenType::LET;
    keywords["fn"] = TokenType::FN;
    keywords["if"] = TokenType::IF;
    keywords["else"] = TokenType::ELSE;
    keywords["while"] = TokenType::WHILE;
    keywords["return"] = TokenType::RETURN;
}

bool Lexer::isAtEnd() const
{
    return current >= source.size();
}

char Lexer::peek() const
{
    if (isAtEnd())
        return '\0';

    return source[current];
}

char Lexer::advance()
{
    return source[current++];
}

Token Lexer::makeToken(
    TokenType type,
    const std::string& lexeme)
{
    return Token(
        type,
        lexeme,
        line);
}

Token Lexer::scanIdentifier()
{
    size_t start = current;

    while (!isAtEnd() &&
        (isalnum(peek()) ||
            peek() == '_'))
    {
        advance();
    }

    std::string text =
        source.substr(
            start,
            current - start);

    auto it =
        keywords.find(text);

    if (it != keywords.end())
    {
        return makeToken(
            it->second,
            text);
    }

    return makeToken(
        TokenType::IDENTIFIER,
        text);
}

Token Lexer::scanNumber()
{
    size_t start = current;

    while (!isAtEnd() &&
        isdigit(peek()))
    {
        advance();
    }

    std::string value =
        source.substr(
            start,
            current - start);

    return makeToken(
        TokenType::NUMBER,
        value);
}

void Lexer::skipWhitespace()
{
    while (!isAtEnd())
    {
        char c = peek();

        if (c == ' ' ||
            c == '\t' ||
            c == '\r')
        {
            advance();
        }
        else if (c == '\n')
        {
            line++;
            advance();
        }
        else
        {
            break;
        }
    }
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (!isAtEnd())
    {
        skipWhitespace();

        if (isAtEnd())
            break;

        char c = peek();

        if (isalpha(c) ||
            c == '_')
        {
            tokens.push_back(
                scanIdentifier());
            continue;
        }

        if (isdigit(c))
        {
            tokens.push_back(
                scanNumber());
            continue;
        }

        advance();

        switch (c)
        {
        case '+':
            tokens.push_back(
                makeToken(
                    TokenType::PLUS,
                    "+"));
            break;

        case '-':
            tokens.push_back(
                makeToken(
                    TokenType::MINUS,
                    "-"));
            break;

        case '*':
            tokens.push_back(
                makeToken(
                    TokenType::STAR,
                    "*"));
            break;

        case '/':
            tokens.push_back(
                makeToken(
                    TokenType::SLASH,
                    "/"));
            break;

        case '=':
            tokens.push_back(
                makeToken(
                    TokenType::ASSIGN,
                    "="));
            break;

        case ';':
            tokens.push_back(
                makeToken(
                    TokenType::SEMICOLON,
                    ";"));
            break;

        case '(':
            tokens.push_back(
                makeToken(
                    TokenType::LPAREN,
                    "("));
            break;

        case ')':
            tokens.push_back(
                makeToken(
                    TokenType::RPAREN,
                    ")"));
            break;

        case '{':
            tokens.push_back(
                makeToken(
                    TokenType::LBRACE,
                    "{"));
            break;

        case '}':
            tokens.push_back(
                makeToken(
                    TokenType::RBRACE,
                    "}"));
            break;

        case '>':
            tokens.push_back(
                makeToken(
                    TokenType::GREATER,
                    ">"));
            break;

        case '<':
            tokens.push_back(
                makeToken(
                    TokenType::LESS,
                    "<"));
            break;

        default:
            tokens.push_back(
                makeToken(
                    TokenType::INVALID,
                    std::string(1, c)));
            break;
        }
    }

    tokens.push_back(
        makeToken(
            TokenType::END_OF_FILE,
            "EOF"));

    return tokens;
}