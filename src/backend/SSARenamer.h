#pragma once

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <vector>
#include <unordered_set>

class SSARenamer
{
public:
    void run(llvm::Function* function);
    void renameBlock(llvm::BasicBlock* block);

private:
    std::unordered_map<std::string, int> versionCounter;

    std::unordered_map<
        std::string,
        std::vector<llvm::Value*>> versionStack;

    std::unordered_set<llvm::BasicBlock*> visited;

    std::unordered_map<
        std::string,
        llvm::Value*> currentValue;
};