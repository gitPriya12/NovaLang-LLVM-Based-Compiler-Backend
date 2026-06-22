#include "DeadCodeElimination.h"

#include "llvm/IR/Instructions.h"

using namespace llvm;

void DeadCodeElimination::run(Function& F)
{
    for (auto& BB : F)
    {
        for (auto it = BB.begin(); it != BB.end(); )
        {
            Instruction* I = &*it++;

            // remove unused instructions
            if (I->use_empty() && !I->isTerminator())
            {
                I->eraseFromParent();
            }
        }
    }
}