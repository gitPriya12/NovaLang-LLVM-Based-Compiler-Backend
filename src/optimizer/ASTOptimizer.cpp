#include "ASTOptimizer.h"

#include "../ast/Program.h"
#include "../ast/LetStmt.h"
#include "../ast/BinaryExpr.h"
#include "../ast/NumberExpr.h"

void ASTOptimizer::optimize(
    Program* program)
{
    for (auto& stmt :
        program->statements)
    {
        auto letStmt =
            dynamic_cast<
            LetStmt*>(
                stmt.get());

        if (!letStmt)
            continue;

        optimizeExpr(
            letStmt->initializer.get());
    }
}


Expr*
ASTOptimizer::optimizeExpr(
    Expr* expr)
{
    auto bin =
        dynamic_cast<
        BinaryExpr*>(expr);

    if (!bin)
        return expr;

}