#include "Parser.h"
#include "../ast/NumberExpr.h"
#include "../ast/VariableExpr.h"
#include "../ast/BinaryExpr.h"
#include "../ast/LetStmt.h"
#include "../ast/Program.h"
#include "../ast/IfStmt.h"
#include "../ast/ComparisonExpr.h"
#include "../ast/WhileStmt.h"
#include "../ast/AssignStmt.h"
#include "../ast/CallExpr.h"
#include "../ast/FunctionStmt.h"
#include "../ast/ReturnStmt.h"


bool check(TokenType type);

Parser::Parser(
    const std::vector<Token>& toks)
    :
    tokens(toks),
    current(0)
{
}

bool Parser::isAtEnd()
{
    return
        tokens[current].type ==
        TokenType::END_OF_FILE;
}

Token Parser::peek()
{
    return tokens[current];
}

Token Parser::advance()
{
    return tokens[current++];
}

bool Parser::match(TokenType type)
{
    if (peek().type == type)
    {
        advance();
        return true;
    }

    return false;
}

std::unique_ptr<Expr>
Parser::parsePrimary()
{
    Token token = advance();

    if (token.type ==
        TokenType::NUMBER)
    {
        return std::make_unique<
            NumberExpr>(
                std::stoi(token.lexeme));
    }

    if (token.type ==
        TokenType::IDENTIFIER)
    {
        return std::make_unique<
            VariableExpr>(
                token.lexeme);
    }

    return nullptr;
}

std::unique_ptr<Expr>
Parser::parseFactor()
{
    auto expr =
        parsePrimary();

    while (true)
    {
        if (match(TokenType::STAR))
        {
            auto right =
                parsePrimary();

            expr =
                std::make_unique<
                BinaryExpr>(
                    '*',
                    std::move(expr),
                    std::move(right));
        }
        else if (match(TokenType::SLASH))
        {
            auto right =
                parsePrimary();

            expr =
                std::make_unique<
                BinaryExpr>(
                    '/',
                    std::move(expr),
                    std::move(right));
        }
        else
        {
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr>
Parser::parseTerm()
{
    auto expr =
        parseFactor();

    while (true)
    {
        if (match(TokenType::PLUS))
        {
            auto right =
                parseFactor();

            expr =
                std::make_unique<
                BinaryExpr>(
                    '+',
                    std::move(expr),
                    std::move(right));
        }
        else if (match(TokenType::MINUS))
        {
            auto right =
                parseFactor();

            expr =
                std::make_unique<
                BinaryExpr>(
                    '-',
                    std::move(expr),
                    std::move(right));
        }
        else
        {
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr>
Parser::parseExpression()
{
    if (check(TokenType::IDENTIFIER) &&
        peekNext().type ==
        TokenType::LPAREN)
    {
        return parseCall();
    }

    return parseTerm();
}

bool Parser::check(TokenType type)
{
    return peek().type == type;
}


std::unique_ptr<Stmt>
Parser::parseLetStatement()
{
    advance();

    Token name =
        advance();

    if (name.type !=
        TokenType::IDENTIFIER)
    {
        return nullptr;
    }

    if (!match(TokenType::ASSIGN))
    {
        return nullptr;
    }

    auto expr =
        parseExpression();

    match(TokenType::SEMICOLON);

    return std::make_unique<
        LetStmt>(
            name.lexeme,
            std::move(expr));
}


std::unique_ptr<Stmt>
Parser::parseStatement()
{
    if (check(TokenType::LET))
    {
        return parseLetStatement();
    }

    if (check(TokenType::IF))
    {
        return parseIfStatement();
    }

    if (check(TokenType::WHILE))
    {
        return parseWhileStatement();
    }

    if (check(TokenType::IDENTIFIER) &&
        peekNext().type ==
        TokenType::ASSIGN)
    {
        return parseAssignment();
    }

    if (check(TokenType::FN))
    {
        return parseFunction();
    }

    if (check(TokenType::RETURN))
    {
        return parseReturn();
    }

    return nullptr;
}

std::unique_ptr<Program>
Parser::parseProgram()
{
    auto program =
        std::make_unique<Program>();

    while (!isAtEnd())
    {
        auto stmt =
            parseStatement();

        if (stmt)
        {
            program->statements
                .push_back(
                    std::move(stmt));
        }
        else
        {
            advance();
        }
    }

    return program;
}


std::unique_ptr<Expr>
Parser::parseComparison()
{
    auto left =
        parseExpression();

    if (match(TokenType::GREATER))
    {
        auto right =
            parseExpression();

        return std::make_unique<
            ComparisonExpr>(
                '>',
                std::move(left),
                std::move(right));
    }

    if (match(TokenType::LESS))
    {
        auto right =
            parseExpression();

        return std::make_unique<
            ComparisonExpr>(
                '<',
                std::move(left),
                std::move(right));
    }

    return left;
}

std::unique_ptr<Stmt>
Parser::parseIfStatement()
{
    advance();

    match(TokenType::LPAREN);

    auto condition =
        parseComparison();

    match(TokenType::RPAREN);

    match(TokenType::LBRACE);

    auto stmt =
        std::make_unique<
        IfStmt>();

    stmt->condition =
        std::move(condition);

    while (!check(
        TokenType::RBRACE))
    {
        auto child =
            parseStatement();

        if (child)
        {
            stmt->thenBranch
                .push_back(
                    std::move(child));
        }
        else
        {
            advance();
        }
    }

    match(TokenType::RBRACE);

    if (match(TokenType::ELSE))
    {
        match(TokenType::LBRACE);

        while (!check(TokenType::RBRACE))
        {
            auto child =
                parseStatement();

            if (child)
            {
                stmt->elseBranch.push_back(
                    std::move(child));
            }
        }

        match(TokenType::RBRACE);
    }

    return stmt;
}

std::unique_ptr<Stmt>
Parser::parseAssignment()
{
    auto stmt =
        std::make_unique<
        AssignStmt>();

    stmt->name =
        advance().lexeme;

    match(TokenType::ASSIGN);

    stmt->value =
        parseExpression();

    match(TokenType::SEMICOLON);

    return stmt;
}

std::unique_ptr<Stmt>
Parser::parseWhileStatement()
{
    advance();

    match(TokenType::LPAREN);

    auto stmt =
        std::make_unique<
        WhileStmt>();

    stmt->condition =
        parseComparison();

    match(TokenType::RPAREN);

    match(TokenType::LBRACE);

    while (!check(
        TokenType::RBRACE))
    {
        auto child =
            parseStatement();

        if (child)
        {
            stmt->body.push_back(
                std::move(child));
        }
    }

    match(TokenType::RBRACE);

    return stmt;
}

Token Parser::peekNext()
{
    if (current + 1 >= tokens.size())
    {
        return tokens.back();
    }

    return tokens[current + 1];
}

std::unique_ptr<Stmt>
Parser::parseFunction()
{
    advance();

    auto fn =
        std::make_unique<
        FunctionStmt>();

    fn->name =
        advance().lexeme;

    match(TokenType::LPAREN);

    while (!check(
        TokenType::RPAREN))
    {
        fn->parameters.push_back(
            advance().lexeme);

        if (check(
            TokenType::COMMA))
        {
            advance();
        }
    }

    match(TokenType::RPAREN);

    match(TokenType::LBRACE);

    while (!check(
        TokenType::RBRACE))
    {
        auto stmt =
            parseStatement();

        if (stmt)
        {
            fn->body.push_back(
                std::move(stmt));
        }
    }

    match(TokenType::RBRACE);

    return fn;
}

std::unique_ptr<Stmt>
Parser::parseReturn()
{
    advance();

    auto stmt =
        std::make_unique<
        ReturnStmt>();

    stmt->value =
        parseExpression();

    match(TokenType::SEMICOLON);

    return stmt;
}

std::unique_ptr<Expr>
Parser::parseCall()
{
    auto call =
        std::make_unique<
        CallExpr>();

    call->callee =
        advance().lexeme;

    match(TokenType::LPAREN);

    while (!check(
        TokenType::RPAREN))
    {
        call->arguments.push_back(
            parseExpression());

        if (check(
            TokenType::COMMA))
        {
            advance();
        }
    }

    match(TokenType::RPAREN);

    return call;
}