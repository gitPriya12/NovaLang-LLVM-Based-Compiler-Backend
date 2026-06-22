#pragma once

#include <memory>
#include <string>

#include "Stmt.h"
#include "Expr.h"

class LetStmt : public Stmt
{
public:

    std::string name;

    std::unique_ptr<Expr> initializer;

    LetStmt(
        const std::string& n,
        std::unique_ptr<Expr> init)
        :
        name(n),
        initializer(std::move(init))
    {
    }
};