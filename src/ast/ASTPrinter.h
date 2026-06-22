#pragma once

#include <iostream>

#include "NumberExpr.h"
#include "VariableExpr.h"
#include "BinaryExpr.h"

class ASTPrinter
{
public:

    static void print(
        Expr* expr,
        int indent = 0)
    {
        if (!expr)
            return;

        std::string pad(
            indent,
            ' ');

        if (auto num =
            dynamic_cast<
                NumberExpr*>(expr))
        {
            std::cout
                << pad
                << "Number("
                << num->value
                << ")\n";
        }

        else if (auto var =
            dynamic_cast<
                VariableExpr*>(expr))
        {
            std::cout
                << pad
                << "Variable("
                << var->name
                << ")\n";
        }

        else if (auto bin =
            dynamic_cast<
                BinaryExpr*>(expr))
        {
            std::cout
                << pad
                << "Binary("
                << bin->op
                << ")\n";

            print(
                bin->left.get(),
                indent + 4);

            print(
                bin->right.get(),
                indent + 4);
        }
    }
};