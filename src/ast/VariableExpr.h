#pragma once

#include <string>

#include "Expr.h"

class VariableExpr : public Expr
{
public:

    std::string name;


    VariableExpr(
        const std::string& n)
        : name(n)
    {
    }


};