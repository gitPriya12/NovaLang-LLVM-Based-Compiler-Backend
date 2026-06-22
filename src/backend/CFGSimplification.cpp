#include "CFGSimplification.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"

using namespace llvm;

void CFGSimplification::run(Function& F)
{
    for (auto& BB : F)
    {
        Instruction* term = BB.getTerminator();

        if (auto* br = dyn_cast<BranchInst>(term))
        {
            // if constant condition branch → simplify
            if (br->isConditional())
            {
                if (auto* cond = dyn_cast<ConstantInt>(br->getCondition()))
                {
                    BasicBlock* target =
                        cond->isOne() ?
                        br->getSuccessor(0) :
                        br->getSuccessor(1);

                    BranchInst::Create(target, &BB);
                    br->eraseFromParent();
                    break;
                }
            }
        }
    }
}