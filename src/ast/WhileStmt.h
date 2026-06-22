#pragma once

#include <memory>
#include <vector>

#include "Stmt.h"

class Expr;

class WhileStmt :
    public Stmt
{
public:

    std::unique_ptr<Expr>
        condition;

    std::vector<
        std::unique_ptr<Stmt>>
        body;
};                                                               