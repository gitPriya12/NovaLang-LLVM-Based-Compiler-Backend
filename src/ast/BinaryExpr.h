#pragma once

#include <memory>

#include "Expr.h"

class BinaryExpr : public Expr
{
public:

    char op;

    std::unique_ptr<Expr> left;

    std::unique_ptr<Expr> right;

    BinaryExpr(
        char oper,
        std::unique_ptr<Expr> lhs,
        std::unique_ptr<Expr> rhs)
        :
        op(oper),
        left(std::move(lhs)),
        right(std::move(rhs))
    {
    }
};