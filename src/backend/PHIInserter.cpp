#include "PHIInserter.h"

#include <iostream>

void PHIInserter::insert(
    llvm::Function* function,
    const std::unordered_map<
    llvm::BasicBlock*,
    std::set<llvm::BasicBlock*>>&df)
{
    std::cout
        << "\n===== PHI Scan =====\n";

    for (auto& bb : *function)
    {
        llvm::BasicBlock* block =
            &bb;

        auto it =
            df.find(block);

        if (it ==
            df.end())
        {
            continue;
        }

        for (auto* frontier :
            it->second)
        {
            std::cout
                << "Need PHI in "
                << frontier
                ->getName()
                .str()
                << "\n";
        }
    }
}