#include "IDominatorTree.h"

#include <iostream>
#include <unordered_set>

#include "llvm/IR/CFG.h"

static bool dominates(
    llvm::BasicBlock* candidate,
    llvm::BasicBlock* block,
    const std::unordered_map<
    llvm::BasicBlock*,
    std::unordered_set<
    llvm::BasicBlock*>>&doms)
{
    auto it =
        doms.find(block);

    if (it == doms.end())
    {
        return false;
    }

    return
        it->second.count(
            candidate);
}

static std::unordered_map<
    llvm::BasicBlock*,
    std::unordered_set<
    llvm::BasicBlock*>>
    computeDominators(
        llvm::Function* function)
{
    std::unordered_map<
        llvm::BasicBlock*,
        std::unordered_set<
        llvm::BasicBlock*>>
        doms;

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

    for (auto* block : blocks)
    {
        if (block == entry)
        {
            doms[block]
                .insert(block);
        }
        else
        {
            for (auto* other :
                blocks)
            {
                doms[block]
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
                        doms[pred];

                    firstPred =
                        false;
                }
                else
                {
                    std::unordered_set<
                        llvm::BasicBlock*>
                        tmp;

                    for (auto* b :
                        newDom)
                    {
                        if (doms[pred]
                            .count(b))
                        {
                            tmp.insert(b);
                        }
                    }

                    newDom =
                        std::move(tmp);
                }
            }

            newDom.insert(
                block);

            if (newDom !=
                doms[block])
            {
                doms[block] =
                    std::move(newDom);

                changed =
                    true;
            }
        }
    }

    return doms;
}

void IDominatorTree::compute(
    llvm::Function* function)
{
    idom.clear();

    auto doms =
        computeDominators(
            function);

    llvm::BasicBlock* entry =
        &function->getEntryBlock();

    idom[entry] =
        nullptr;

    for (auto& bb :
        *function)
    {
        llvm::BasicBlock* block =
            &bb;

        if (block == entry)
        {
            continue;
        }

        llvm::BasicBlock*
            immediate =
            nullptr;

        for (auto* candidate :
            doms[block])
        {
            if (candidate ==
                block)
            {
                continue;
            }

            bool closest =
                true;

            for (auto* other :
                doms[block])
            {
                if (other ==
                    block ||
                    other ==
                    candidate)
                {
                    continue;
                }

                if (dominates(
                    candidate,
                    other,
                    doms))
                {
                    closest =
                        false;

                    break;
                }
            }

            if (closest)
            {
                immediate =
                    candidate;

                break;
            }
        }

        idom[block] =
            immediate;
    }
}

void IDominatorTree::print()
{
    std::cout
        << "\n===== IDOM =====\n";

    for (auto& pair :
        idom)
    {
        std::cout
            << pair.first
            ->getName()
            .str()
            << " <- ";

        if (pair.second)
        {
            std::cout
                << pair.second
                ->getName()
                .str();
        }
        else
        {
            std::cout
                << "null";
        }

        std::cout
            << "\n";
    }
}


llvm::BasicBlock*
IDominatorTree::getIDom(
    llvm::BasicBlock* block)
{
    auto it =
        idom.find(block);

    if (it ==
        idom.end())
    {
        return nullptr;
    }

    return it->second;
}


