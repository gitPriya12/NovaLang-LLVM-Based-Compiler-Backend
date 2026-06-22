#pragma once

#include <memory>

#include "Stmt.h"

class Expr;

class AssignStmt :
    public Stmt
{
public:

    std::string name;

    std::unique_ptr<Expr>
        value;
};