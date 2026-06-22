#pragma once

#include <vector>

#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"

struct CFGNode
{
    llvm::BasicBlock* block;

    std::vector<CFGNode*> successors;

    std::vector<CFGNode*> predecessors;
};

class CFGBuilder
{
private:

    std::vector<CFGNode> nodes;

public:

    void build(
        llvm::Function* function);

    void print();

private:

    CFGNode* findNode(
        llvm::BasicBlock* block);
};