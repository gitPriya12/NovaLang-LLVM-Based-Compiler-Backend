#include "LLVMCodeGen.h"

#include "llvm/Support/raw_ostream.h"

#include "../ast/NumberExpr.h"
#include "../ast/LetStmt.h"
#include "../ast/VariableExpr.h"
#include "../ast/Program.h"
#include "../ast/ComparisonExpr.h"
#include "../ast/IfStmt.h"
#include "../ast/AssignStmt.h"
#include "../ast/WhileStmt.h"
#include "../ast/FunctionStmt.h"
#include "../ast/ReturnStmt.h"
#include "../ast/CallExpr.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/PassManager.h"

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

#include "../backend/OptimizationPipeline.h"

#include <iostream>

// ================= CONSTRUCTOR =================

LLVMCodeGen::LLVMCodeGen()
    : builder(context)
{
    module = std::make_unique<llvm::Module>("NovaModule", context);
}

void LLVMCodeGen::printIR()
{
    module->print(llvm::outs(), nullptr);
}

// ================= SAFE HELPER =================

bool LLVMCodeGen::currentBlockHasTerminator()
{
    auto* BB = builder.GetInsertBlock();
    return BB && BB->getTerminatorOrNull();
}

// ================= MAIN =================

void LLVMCodeGen::generate(Program* program)
{
    llvm::FunctionType* mainType =
        llvm::FunctionType::get(builder.getInt32Ty(), false);

    llvm::Function* mainFunc =
        llvm::Function::Create(
            mainType,
            llvm::Function::ExternalLinkage,
            "main",
            module.get());

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(context, "entry", mainFunc);

    builder.SetInsertPoint(entry);

    namedValuesStack.push_back({});

    for (auto& stmt : program->statements)
        generateStmt(stmt.get());

    // SAFE RETURN (no raw getTerminator)
    if (!currentBlockHasTerminator())
        builder.CreateRet(builder.getInt32(0));

    namedValuesStack.pop_back();
}

// ================= ALLOCA =================

llvm::AllocaInst* LLVMCodeGen::createEntryBlockAlloca(
    llvm::Function* function,
    const std::string& name)
{
    llvm::IRBuilder<> tmp(
        &function->getEntryBlock(),
        function->getEntryBlock().begin());

    return tmp.CreateAlloca(builder.getInt32Ty(), nullptr, name);
}

// ================= EXPRESSIONS =================

llvm::Value* LLVMCodeGen::generateExpr(Expr* expr)
{
    if (auto num = dynamic_cast<NumberExpr*>(expr))
        return builder.getInt32(num->value);

    if (auto var = dynamic_cast<VariableExpr*>(expr))
    {
        auto& scope = namedValuesStack.back();
        auto it = scope.find(var->name);
        if (it == scope.end()) return nullptr;

        return builder.CreateLoad(builder.getInt32Ty(), it->second);
    }

    if (auto bin = dynamic_cast<BinaryExpr*>(expr))
    {
        auto L = generateExpr(bin->left.get());
        auto R = generateExpr(bin->right.get());
        if (!L || !R) return nullptr;

        switch (bin->op)
        {
        case '+': return builder.CreateAdd(L, R);
        case '-': return builder.CreateSub(L, R);
        case '*': return builder.CreateMul(L, R);
        case '/': return builder.CreateSDiv(L, R);
        }
    }

    if (auto cmp = dynamic_cast<ComparisonExpr*>(expr))
    {
        auto L = generateExpr(cmp->left.get());
        auto R = generateExpr(cmp->right.get());
        if (!L || !R) return nullptr;

        if (cmp->op == '>') return builder.CreateICmpSGT(L, R);
        if (cmp->op == '<') return builder.CreateICmpSLT(L, R);
    }

    if (auto call = dynamic_cast<CallExpr*>(expr))
    {
        auto it = functions.find(call->callee);
        if (it == functions.end()) return nullptr;

        llvm::Function* fn = it->second;

        std::vector<llvm::Value*> args;
        for (auto& a : call->arguments)
            args.push_back(generateExpr(a.get()));

        return builder.CreateCall(fn, args);
    }

    return nullptr;
}

// ================= STATEMENTS =================

void LLVMCodeGen::generateStmt(Stmt* stmt)
{
    auto* BB = builder.GetInsertBlock();
    if (!BB) return;

    // IMPORTANT: do NOT blindly skip logic here
    // (this was your original crash source)

    // LET
    if (auto letStmt = dynamic_cast<LetStmt*>(stmt))
    {
        auto val = generateExpr(letStmt->initializer.get());
        if (!val) return;

        auto fn = BB->getParent();
        auto alloca = createEntryBlockAlloca(fn, letStmt->name);

        builder.CreateStore(val, alloca);
        namedValuesStack.back()[letStmt->name] = alloca;
        return;
    }

    // IF
    if (auto ifStmt = dynamic_cast<IfStmt*>(stmt))
    {
        auto fn = BB->getParent();

        auto thenBB = llvm::BasicBlock::Create(context, "then", fn);
        auto elseBB = llvm::BasicBlock::Create(context, "else", fn);
        auto mergeBB = llvm::BasicBlock::Create(context, "merge", fn);

        auto cond = generateExpr(ifStmt->condition.get());
        if (!cond) return;

        builder.CreateCondBr(cond, thenBB, elseBB);

        // THEN
        builder.SetInsertPoint(thenBB);
        for (auto& s : ifStmt->thenBranch)
            generateStmt(s.get());

        if (!currentBlockHasTerminator())
            builder.CreateBr(mergeBB);

        // ELSE
        builder.SetInsertPoint(elseBB);
        for (auto& s : ifStmt->elseBranch)
            generateStmt(s.get());

        if (!currentBlockHasTerminator())
            builder.CreateBr(mergeBB);

        builder.SetInsertPoint(mergeBB);
        return;
    }

    // ASSIGN
    if (auto as = dynamic_cast<AssignStmt*>(stmt))
    {
        auto val = generateExpr(as->value.get());
        if (!val) return;

        auto& scope = namedValuesStack.back();
        auto it = scope.find(as->name);
        if (it == scope.end()) return;

        builder.CreateStore(val, it->second);
        return;
    }

    // WHILE
    if (auto loop = dynamic_cast<WhileStmt*>(stmt))
    {
        auto fn = BB->getParent();

        auto condBB = llvm::BasicBlock::Create(context, "cond", fn);
        auto bodyBB = llvm::BasicBlock::Create(context, "body", fn);
        auto endBB = llvm::BasicBlock::Create(context, "end", fn);

        builder.CreateBr(condBB);

        // COND
        builder.SetInsertPoint(condBB);
        auto cond = generateExpr(loop->condition.get());
        if (!cond) return;

        builder.CreateCondBr(cond, bodyBB, endBB);

        // BODY
        builder.SetInsertPoint(bodyBB);
        for (auto& s : loop->body)
            generateStmt(s.get());

        if (!currentBlockHasTerminator())
            builder.CreateBr(condBB);

        builder.SetInsertPoint(endBB);
        return;
    }

    // FUNCTION
    if (auto fnStmt = dynamic_cast<FunctionStmt*>(stmt))
    {
        generateFunction(fnStmt);
        return;
    }

    // RETURN
    if (auto ret = dynamic_cast<ReturnStmt*>(stmt))
    {
        auto val = generateExpr(ret->value.get());
        if (!val) return;

        if (!currentBlockHasTerminator())
            builder.CreateRet(val);

        return;
    }
}

// ================= FUNCTION =================

void LLVMCodeGen::generateFunction(FunctionStmt* fn)
{
    std::vector<llvm::Type*> params(
        fn->parameters.size(),
        builder.getInt32Ty());

    auto fnType =
        llvm::FunctionType::get(builder.getInt32Ty(), params, false);

    auto function =
        llvm::Function::Create(
            fnType,
            llvm::Function::ExternalLinkage,
            fn->name,
            module.get());

    functions[fn->name] = function;

    auto block =
        llvm::BasicBlock::Create(context, "entry", function);

    builder.SetInsertPoint(block);

    namedValuesStack.push_back({});

    int i = 0;
    for (auto& arg : function->args())
    {
        arg.setName(fn->parameters[i]);

        auto alloca = createEntryBlockAlloca(function, fn->parameters[i]);
        builder.CreateStore(&arg, alloca);

        namedValuesStack.back()[fn->parameters[i]] = alloca;
        i++;
    }

    for (auto& stmt : fn->body)
        generateStmt(stmt.get());

    if (!currentBlockHasTerminator())
        builder.CreateRet(builder.getInt32(0));

    namedValuesStack.pop_back();
}

// ================= OPTIMIZATION =================

void LLVMCodeGen::optimize()
{
    OptimizationPipeline pipeline;
    pipeline.run(*module);
}