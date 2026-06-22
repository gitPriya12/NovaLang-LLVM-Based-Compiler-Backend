#include "ConstantFolding.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"

using namespace llvm;

void ConstantFolding::run(Function& F)
{
    for (auto& BB : F)
    {
        for (auto it = BB.begin(); it != BB.end(); )
        {
            Instruction* I = &*it++;

            if (auto* binOp = dyn_cast<BinaryOperator>(I))
            {
                Value* L = binOp->getOperand(0);
                Value* R = binOp->getOperand(1);

                auto* LC = dyn_cast<ConstantInt>(L);
                auto* RC = dyn_cast<ConstantInt>(R);

                if (LC && RC)
                {
                    Constant* folded =
                        ConstantExpr::get(binOp->getOpcode(), LC, RC);

                    binOp->replaceAllUsesWith(folded);
                    binOp->eraseFromParent();
                }
            }
        }
    }
}