#pragma once

#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Value.h"

#include <unordered_map>
#include <set>
#include <string>

class PHIBuilder
{
public:
    void run(
        llvm::Function* function,
        const std::unordered_map<
        llvm::BasicBlock*,
        std::set<llvm::BasicBlock*>>&df,

        const std::unordered_map<
        llvm::BasicBlock*,
        std::unordered_map<std::string, llvm::Value*>>&blockDefs
    );
};