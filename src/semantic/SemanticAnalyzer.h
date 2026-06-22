#pragma once

#include <vector>

#include "SymbolTable.h"

#include "../ast/Program.h"
#include "../ast/Expr.h"
#include <unordered_set>

class SemanticAnalyzer
{
private:

    std::vector<SymbolTable>
        scopes;

    std::unordered_set<std::string>
        functions;

    void analyzeStmt(
        Stmt* stmt);

    void analyzeExpr(
        Expr* expr);

    void enterScope();

    void leaveScope();

    bool exists(
        const std::string& name);


    std::unique_ptr<Expr>
        foldConstants(
            std::unique_ptr<Expr> expr);

    void removeDeadCode(
        Program* program);
public:

    void analyze(
        Program* program);
};