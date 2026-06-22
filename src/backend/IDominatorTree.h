#pragma once

#include <unordered_map>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"

class IDominatorTree
{
private:

    std::unordered_map<
        llvm::BasicBlock*,
        llvm::BasicBlock*>
        idom;

public:

    void compute(
        llvm::Function* function);

    void print();

    llvm::BasicBlock*
        getIDom(
            llvm::BasicBlock* block);
};