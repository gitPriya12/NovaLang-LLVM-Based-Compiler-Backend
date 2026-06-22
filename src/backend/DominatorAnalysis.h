#pragma once

#include <unordered_map>
#include <unordered_set>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"

class DominatorAnalysis
{
private:

    std::unordered_map<
        llvm::BasicBlock*,
        std::unordered_set<
        llvm::BasicBlock*>>
        dominators;

public:

    void compute(
        llvm::Function* function);

    void print();

    const auto&
        getDominators() const
    {
        return dominators;
    }
};