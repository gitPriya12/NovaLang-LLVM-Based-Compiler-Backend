#include "OptimizationPipeline.h"

// Custom passes
#include "ConstantFolding.h"
#include "DeadCodeElimination.h"
#include "CopyPropagation.h"
#include "CFGSimplification.h"

// LLVM
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

#include <iostream>

void OptimizationPipeline::run(llvm::Module& module)
{
    std::cout << "\n===== RUNNING FULL OPTIMIZATION PIPELINE =====\n";

    // =====================================================
    // PHASE 1: CUSTOM PASSES (FUNCTION LEVEL)
    // =====================================================
    std::cout << "\n--- Running Custom Compiler Passes ---\n";

    ConstantFolding cf;
    DeadCodeElimination dce;
    CopyPropagation cp;
    CFGSimplification cfg;

    for (llvm::Function& F : module)
    {
        if (F.isDeclaration())
            continue;

        cf.run(F);
        dce.run(F);
        cp.run(F);
        cfg.run(F);
    }

    std::cout << "\n--- Custom Passes Completed ---\n";

    // =====================================================
    // PHASE 2: LLVM OPTIMIZATION PIPELINE
    // =====================================================
    llvm::PassBuilder pb;

    llvm::LoopAnalysisManager lam;
    llvm::FunctionAnalysisManager fam;
    llvm::CGSCCAnalysisManager cgam;
    llvm::ModuleAnalysisManager mam;

    pb.registerModuleAnalyses(mam);
    pb.registerCGSCCAnalyses(cgam);
    pb.registerFunctionAnalyses(fam);
    pb.registerLoopAnalyses(lam);

    pb.crossRegisterProxies(lam, fam, cgam, mam);

    llvm::ModulePassManager mpm;

    mpm.addPass(
        llvm::createModuleToFunctionPassAdaptor(
            llvm::InstCombinePass()));

    mpm.addPass(
        llvm::createModuleToFunctionPassAdaptor(
            llvm::ReassociatePass()));

    mpm.addPass(
        llvm::createModuleToFunctionPassAdaptor(
            llvm::GVNPass()));

    mpm.addPass(
        llvm::createModuleToFunctionPassAdaptor(
            llvm::SimplifyCFGPass()));

    mpm.run(module, mam);

    std::cout << "\n===== LLVM OPTIMIZATION DONE =====\n";
}