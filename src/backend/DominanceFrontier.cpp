#include "DominanceFrontier.h"

#include "IDominatorTree.h"

#include <iostream>

#include "llvm/IR/CFG.h"

void DominanceFrontier::compute(
    llvm::Function* function,
    IDominatorTree& idomTree)
{
    frontier.clear();

    for (auto& bb : *function)
    {
        llvm::BasicBlock* block =
            &bb;

        unsigned predCount = 0;

        for (auto pred :
            llvm::predecessors(
                block))
        {
            predCount++;
        }

        if (predCount < 2)
        {
            continue;
        }

        for (auto pred :
            llvm::predecessors(
                block))
        {
            llvm::BasicBlock* runner =
                pred;

            while (runner &&
                runner !=
                idomTree.getIDom(
                    block))
            {
                frontier[runner]
                    .insert(block);

                runner =
                    idomTree.getIDom(
                        runner);
            }
        }
    }
}

void DominanceFrontier::print()
{
    std::cout
        << "\n===== Dominance Frontier =====\n";

    for (auto& pair :
        frontier)
    {
        std::cout
            << pair.first
            ->getName()
            .str()
            << " : ";

        for (auto* block :
            pair.second)
        {
            std::cout
                << block
                ->getName()
                .str()
                << " ";
        }

        std::cout
            << "\n";
    }
}