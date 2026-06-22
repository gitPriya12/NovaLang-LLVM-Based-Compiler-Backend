#include "DominatorTree.h"
#include "IDominatorTree.h"

#include <iostream>

void DominatorTree::build(
    llvm::Function* function,
    IDominatorTree& idomTree)
{
    children.clear();

    for (auto& bb : *function)
    {
        llvm::BasicBlock* block =
            &bb;

        llvm::BasicBlock* parent =
            idomTree.getIDom(
                block);

        if (!parent)
        {
            continue;
        }

        children[parent]
            .push_back(block);
    }
}


void DominatorTree::print()
{
    std::cout
        << "\n===== Dominator Tree =====\n";

    for (auto& pair :
        children)
    {
        std::cout
            << pair.first
            ->getName()
            .str()
            << " -> ";

        for (auto* child :
            pair.second)
        {
            std::cout
                << child->getName().str()
                << " ";
        }

        std::cout
            << "\n";
    }
}


