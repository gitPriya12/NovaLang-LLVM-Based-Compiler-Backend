#pragma once

#include <unordered_map>
#include <set>

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"

class PHIInserter
{
public:

    void insert(
        llvm::Function* function,
        const std::unordered_map<
        llvm::BasicBlock*,
        std::set<llvm::BasicBlock*>>&df);
};