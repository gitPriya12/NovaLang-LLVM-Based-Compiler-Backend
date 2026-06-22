#pragma once

#include "Expr.h"

class NumberExpr : public Expr
{
public:

    int value;

    NumberExpr(int v)
        : value(v)
    {
    }
};