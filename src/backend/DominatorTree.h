#pragma once

#include <unordered_map>
#include <vector>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"

class IDominatorTree;

class DominatorTree
{
private:

    std::unordered_map<
        llvm::BasicBlock*,
        std::vector<
        llvm::BasicBlock*>>
        children;

public:

    void build(
        llvm::Function* function,
        IDominatorTree& idom);

    void print();
};