#pragma once

class Program;
class Expr;

class ASTOptimizer
{
public:

    void optimize(
        Program* program);

private:

    Expr* optimizeExpr(
        Expr* expr);
};