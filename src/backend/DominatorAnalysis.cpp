#include "DominatorAnalysis.h"

#include <iostream>

#include "llvm/IR/CFG.h"

void DominatorAnalysis::compute(
    llvm::Function* function)
{
    dominators.clear();

    llvm::BasicBlock* entry =
        &function->getEntryBlock();

    std::vector<
        llvm::BasicBlock*>
        blocks;

    for (auto& bb : *function)
    {
        blocks.push_back(
            &bb);
    }

    //
    // Initialization
    //

    for (auto* block : blocks)
    {
        if (block == entry)
        {
            dominators[block]
                .insert(block);
        }
        else
        {
            for (auto* other :
                blocks)
            {
                dominators[block]
                    .insert(other);
            }
        }
    }

    bool changed = true;

    while (changed)
    {
        changed = false;

        for (auto* block :
            blocks)
        {
            if (block == entry)
            {
                continue;
            }

            std::unordered_set<
                llvm::BasicBlock*>
                newDom;

            bool firstPred =
                true;

            for (auto pred :
                llvm::predecessors(
                    block))
            {
                if (firstPred)
                {
                    newDom =
                        dominators[pred];

                    firstPred =
                        false;
                }
                else
                {
                    std::unordered_set<
                        llvm::BasicBlock*>
                        intersection;

                    for (auto* b :
                        newDom)
                    {
                        if (dominators[pred]
                            .count(b))
                        {
                            intersection
                                .insert(b);
                        }
                    }

                    newDom =
                        std::move(
                            intersection);
                }
            }

            newDom.insert(
                block);

            if (newDom !=
                dominators[block])
            {
                dominators[block] =
                    std::move(
                        newDom);

                changed =
                    true;
            }
        }
    }
}

void DominatorAnalysis::print()
{
    std::cout
        << "\n===== Dominators =====\n";

    for (auto& pair :
        dominators)
    {
        std::cout
            << pair.first
            ->getName()
            .str()
            << " : ";

        for (auto* dom :
            pair.second)
        {
            std::cout
                << dom->getName()
                .str()
                << " ";
        }

        std::cout
            << "\n";
    }
}