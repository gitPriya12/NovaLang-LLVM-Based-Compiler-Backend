#include "SemanticAnalyzer.h"

#include "../ast/LetStmt.h"
#include "../ast/BinaryExpr.h"
#include "../ast/VariableExpr.h"
#include "../ast/NumberExpr.h"
#include "../ast/IfStmt.h"
#include "Type.h"
#include "../ast/FunctionStmt.h"
#include "../ast/Program.h"
#include "../ast/CallExpr.h"
#include "../ast/ReturnStmt.h"
#include "../ast/BoolExpr.h"
#include "../ast/WhileStmt.h"
#include "../ast/ComparisonExpr.h"
#include <iostream>
#include <utility>


void removeDeadCode(
    Program* program);

void SemanticAnalyzer::analyze(
    Program* program)
{
    enterScope();

    for (auto& stmt :
        program->statements)
    {
        analyzeStmt(
            stmt.get());
    }

    leaveScope();

    removeDeadCode(
        program);
}



void SemanticAnalyzer::analyzeStmt(
    Stmt* stmt)
{
    if (auto letStmt =
        dynamic_cast<LetStmt*>(stmt))
    {
        analyzeExpr(
            letStmt->initializer.get());

        scopes.back().insert(
            letStmt->name,
            Type::INT);

        return;
    }

    if (auto ifStmt =
        dynamic_cast<IfStmt*>(stmt))
    {
        analyzeExpr(
            ifStmt->condition.get());

        enterScope();

        for (auto& child :
            ifStmt->thenBranch)
        {
            analyzeStmt(
                child.get());
        }

        leaveScope();

        enterScope();

        for (auto& child :
            ifStmt->elseBranch)
        {
            analyzeStmt(
                child.get());
        }

        leaveScope();

        return;
    }

    if (auto fn =
        dynamic_cast<FunctionStmt*>(stmt))
    {

        scopes.front().insert(
            fn->name,
            Type::FUNCTION);

        enterScope();

        for (auto& param :
            fn->parameters)
        {
            scopes.back().insert(
                param,
                Type::INT);
        }

        for (auto& child :
            fn->body)
        {
            analyzeStmt(
                child.get());
        }

        leaveScope();

        return;
    }

    if (auto whileStmt =
        dynamic_cast<
        WhileStmt*>(stmt))
    {
        analyzeExpr(
            whileStmt->condition.get());

        enterScope();

        for (auto& child :
            whileStmt->body)
        {
            analyzeStmt(
                child.get());
        }

        leaveScope();

        return;
    }
}


void SemanticAnalyzer::analyzeExpr(
    Expr* expr)
{
    if (!expr)
        return;

    if (auto var =
        dynamic_cast<
        VariableExpr*>(expr))
    {
        if (functions.count(
            var->name))
        {
            return;
        }

        bool found = false;

        for (auto it =
            scopes.rbegin();
            it != scopes.rend();
            ++it)
        {
            if (it->exists(
                var->name))
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            std::cout
                << "undefined variable "
                << var->name
                << "\n";
        }

        return;
    }

    auto cmp =
        dynamic_cast<
        ComparisonExpr*>(
            expr);

    if (cmp)
    {
        cmp->left =
            foldConstants(
                std::move(
                    cmp->left));

        cmp->right =
            foldConstants(
                std::move(
                    cmp->right));

        auto leftNum =
            dynamic_cast<
            NumberExpr*>(
                cmp->left.get());

        auto rightNum =
            dynamic_cast<
            NumberExpr*>(
                cmp->right.get());

        if (leftNum &&
            rightNum)
        {
            return;
        }
    }

    auto bin =
        dynamic_cast<
        BinaryExpr*>(expr);

    if (bin)
    {
        analyzeExpr(
            bin->left.get());

        analyzeExpr(
            bin->right.get());

        return;
    }

    if (auto call =
        dynamic_cast<CallExpr*>(expr))
    {
        for (auto& arg :
            call->arguments)
        {
            analyzeExpr(
                arg.get());
        }

        return;
    }
}

void SemanticAnalyzer::enterScope()
{
    scopes.push_back(
        SymbolTable());

   
}

void SemanticAnalyzer::leaveScope()
{
    scopes.pop_back();
}

bool SemanticAnalyzer::exists(
    const std::string& name)
{
    for (auto it =
        scopes.rbegin();
        it != scopes.rend();
        ++it)
    {
        if (it->exists(name))
        {
            return true;
        }
    }

    return false;
}

std::unique_ptr<Expr>
SemanticAnalyzer::foldConstants(
    std::unique_ptr<Expr> expr)
{
    auto bin =
        dynamic_cast<
        BinaryExpr*>(
            expr.get());

    if (!bin)
    {
        return expr;
    }

    bin->left =
        foldConstants(
            std::move(
                bin->left));

    bin->right =
        foldConstants(
            std::move(
                bin->right));

    auto leftNum =
        dynamic_cast<
        NumberExpr*>(
            bin->left.get());

    auto rightNum =
        dynamic_cast<
        NumberExpr*>(
            bin->right.get());

    if (!leftNum ||
        !rightNum)
    {
        return expr;
    }

    int result = 0;

    switch (bin->op)
    {
    case '+':
        result =
            leftNum->value +
            rightNum->value;
        break;

    case '-':
        result =
            leftNum->value -
            rightNum->value;
        break;

    case '*':
        result =
            leftNum->value *
            rightNum->value;
        break;

    case '/':
        result =
            leftNum->value /
            rightNum->value;
        break;

    default:
        return expr;
    }

    return std::make_unique<
        NumberExpr>(
            result);
}

void SemanticAnalyzer::
removeDeadCode(
    Program* program)
{
    bool foundReturn =
        false;

    std::vector<
        std::unique_ptr<Stmt>>
        result;

    for (auto& stmt :
        program->statements)
    {
        if (foundReturn)
        {
            continue;
        }

        result.push_back(
            std::move(stmt));

        if (dynamic_cast<
            ReturnStmt*>(
                result.back().get()))
        {
            foundReturn =
                true;
        }
    }

    program->statements =
        std::move(result);
}
