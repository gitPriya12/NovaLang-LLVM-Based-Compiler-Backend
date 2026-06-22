#pragma once

#include <memory>

#include "Stmt.h"
#include "Expr.h"

class ExpressionStmt : public Stmt
{
public:

    std::unique_ptr<Expr> expr;

    ExpressionStmt(
        std::unique_ptr<Expr> e)
        :
        expr(std::move(e))
    {
    }
};