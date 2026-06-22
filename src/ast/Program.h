#pragma once

#include <memory>
#include <vector>

#include "Stmt.h"

class Program
{
public:

    std::vector<
        std::unique_ptr<Stmt>
    > statements;
};