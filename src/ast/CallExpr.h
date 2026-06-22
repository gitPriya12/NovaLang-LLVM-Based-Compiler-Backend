#pragma once

#include <memory>
#include <vector>

#include "Expr.h"

class CallExpr :
    public Expr
{
public:

    std::string callee;

    std::vector<
        std::unique_ptr<Expr>>
        arguments;
};