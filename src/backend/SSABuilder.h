#pragma once

#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/BasicBlock.h"

#include <unordered_map>
#include <vector>
#include <set>
#include <string>

class SSABuilder
{
public:
    void build(
        llvm::Function& function,
        const std::unordered_map<
        llvm::BasicBlock*,
        std::set<llvm::BasicBlock*>>&df);

    void pushVersion(const std::string& name, llvm::Value* v);
    llvm::Value* getCurrentVersion(const std::string& name);
    llvm::Value* createNewVersion(const std::string& name, llvm::Value* v);

    std::unordered_map<
        llvm::BasicBlock*,
        std::unordered_map<std::string, llvm::Value*>
    > blockDefs;


    const auto& getBlockDefs() const { return blockDefs; }

private:
    std::unordered_map<std::string, std::vector<llvm::Value*>> versionStack;
    std::unordered_map<std::string, int> versionCounter;


};