#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Stmt.h"

class Stmt;

class FunctionStmt :
    public Stmt
{
public:

    std::string name;

    std::vector<std::string>
        parameters;

    std::vector<
        std::unique_ptr<Stmt>>
        body;
};