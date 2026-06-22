#include "CFGBuilder.h"

CFGNode* CFGBuilder::findNode(llvm::BasicBlock* block)
{
    for (auto& node : nodes)
    {
        if (node.block == block)
        {
            return &node;
        }
    }

    return nullptr;
}

void CFGBuilder::build(llvm::Function* function)
{
    nodes.clear();

    for (auto& bb : *function)
    {
        CFGNode node;
        node.block = &bb;
        nodes.push_back(std::move(node));
    }

    for (auto& bb : *function)
    {
        CFGNode* current = findNode(&bb);

        auto* term = bb.getTerminator();
        if (!term)
            continue;

        for (unsigned i = 0; i < term->getNumSuccessors(); i++)
        {
            auto* succ = term->getSuccessor(i);

            CFGNode* succNode = findNode(succ);

            if (!current || !succNode)
                continue;

            current->successors.push_back(succNode);
            succNode->predecessors.push_back(current);
        }
    }
}