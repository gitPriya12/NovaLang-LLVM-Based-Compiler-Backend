#include "PHINodePass.h"

#include <iostream>

void PHINodePass::run(
    llvm::Function* function)
{
    std::cout
        << "\n===== PHI Scan =====\n";

    for (auto& bb : *function)
    {
        for (auto& inst : bb)
        {
            if (llvm::isa<llvm::PHINode>(
                &inst))
            {
                std::cout
                    << "PHI found in "
                    << bb.getName().str()
                    << "\n";
            }
        }
    }
}