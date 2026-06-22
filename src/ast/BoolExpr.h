#pragma once

#include "Expr.h"

class BoolExpr :
    public Expr
{
public:

    bool value;

    BoolExpr(
        bool v)
        :
        value(v)
    {
    }
};