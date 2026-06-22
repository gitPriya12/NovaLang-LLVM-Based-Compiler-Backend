#pragma once

#include <unordered_map>
#include <set>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"

class IDominatorTree;

class DominanceFrontier
{
private:

    std::unordered_map<
        llvm::BasicBlock*,
        std::set<llvm::BasicBlock*>>
        frontier;

public:

    void compute(
        llvm::Function* function,
        IDominatorTree& idomTree);

    void print();

    const std::unordered_map<
        llvm::BasicBlock*,
        std::set<llvm::BasicBlock*>>&
        getFrontiers() const
    {
        return frontier;
    }
};