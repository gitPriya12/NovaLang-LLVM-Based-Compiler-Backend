#pragma once

#include <memory>
#include <vector>

#include "../lexer/Token.h"

#include "../ast/Expr.h"
#include "../ast/Stmt.h"
#include "../ast/Program.h"


class Parser
{
private:

    std::vector<Token> tokens;

    size_t current;

public:

    Parser(
        const std::vector<Token>& toks);

    std::unique_ptr<Expr>
    parseExpression();

    std::unique_ptr<Program>
    parseProgram();

private:

    bool isAtEnd();

    Token peek();

    Token advance();

    bool match(TokenType type);

    bool check(TokenType type);

    std::unique_ptr<Expr>
    parseTerm();

    std::unique_ptr<Expr>
    parseFactor();

    std::unique_ptr<Expr>
    parsePrimary();

    std::unique_ptr<Stmt>
    parseStatement();

    std::unique_ptr<Stmt>
    parseLetStatement();

    std::unique_ptr<Stmt>
        parseIfStatement();

    std::unique_ptr<Expr>
        parseComparison();

    std::unique_ptr<Stmt>
        parseAssignment();

    std::unique_ptr<Stmt>
        parseWhileStatement();

    Token peekNext();

    std::unique_ptr<Stmt>
parseFunction();

std::unique_ptr<Stmt>
parseReturn();

std::unique_ptr<Expr>
parseCall();
};