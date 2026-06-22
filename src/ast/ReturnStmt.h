#pragma once

#include <memory>

#include "Stmt.h"

class Expr;

class ReturnStmt :
    public Stmt
{
public:

    std::unique_ptr<Expr>
        value;
};