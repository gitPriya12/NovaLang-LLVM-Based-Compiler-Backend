#include "CopyPropagation.h"

#include "llvm/IR/Instructions.h"

using namespace llvm;

void CopyPropagation::run(Function& F)
{
    for (auto& BB : F)
    {
        for (auto& I : BB)
        {
            if (auto* store = dyn_cast<StoreInst>(&I))
            {
                Value* val = store->getValueOperand();
                Value* ptr = store->getPointerOperand();

                if (auto* load = dyn_cast<LoadInst>(val))
                {
                    store->setOperand(1, load->getPointerOperand());
                }
            }
        }
    }
}