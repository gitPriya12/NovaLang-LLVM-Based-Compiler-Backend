#pragma once

#include <memory>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include <unordered_map>
#include "../ast/Expr.h"
#include "../ast/BinaryExpr.h"

#include <unordered_map>
#include <string>


class Expr;
class Stmt;
class Program;
class FunctionStmt;


class LLVMCodeGen
{
private:

    llvm::LLVMContext context;

    llvm::IRBuilder<> builder;

    std::unique_ptr<llvm::Module> module;

    std::vector<
        std::unordered_map<std::string, llvm::AllocaInst*>
    > namedValuesStack;

    std::unordered_map<
        std::string,
        llvm::Function*>
        functions;

    llvm::Value*
        generateExpr(
            Expr* expr);

    llvm::AllocaInst*
        createEntryBlockAlloca(
            llvm::Function* function,
            const std::string& name);

    void generateStmt(
        Stmt* stmt);

    void generateFunction(
        FunctionStmt* fn);


    bool isTerminated();

    bool currentBlockHasTerminator();


public:

    LLVMCodeGen();

    void generate(
        Program* program);

    void printIR();

    void optimize();

     llvm::Module*
    getModule()
    {
        return module.get();
    }

};