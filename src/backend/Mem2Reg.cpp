#include "Mem2Reg.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/IRBuilder.h"
#include <iostream>

using namespace llvm;

static std::string getBaseName(const std::string& name)
{
    std::string base;

    for (char c : name)
    {
        if (isdigit(c))
            break;
        base.push_back(c);
    }

    return base;
}

void Mem2Reg::run(Function& F)
{
    std::cout << "\n===== MEM2REG =====\n";

    // STEP 1: traverse blocks
    for (auto& BB : F)
    {
        for (auto it = BB.begin(); it != BB.end(); )
        {
            Instruction* I = &*it++;

            // ================= STORE =================
            if (auto* store = dyn_cast<StoreInst>(I))
            {
                Value* ptr = store->getPointerOperand();
                Value* val = store->getValueOperand();

                if (!ptr->hasName())
                    continue;

                std::string name = getBaseName(ptr->getName().str());

                currentValue[name] = val;

                std::cout << "STORE removed -> " << name << "\n";

                store->eraseFromParent();
            }

            // ================= LOAD =================
            else if (auto* load = dyn_cast<LoadInst>(I))
            {
                Value* ptr = load->getPointerOperand();

                if (!ptr->hasName())
                    continue;

                std::string name = getBaseName(ptr->getName().str());

                if (currentValue.count(name))
                {
                    Value* val = currentValue[name];

                    load->replaceAllUsesWith(val);

                    std::cout << "LOAD replaced -> " << name << "\n";
                }

                load->eraseFromParent();
            }

            // ================= ALLOCA =================
            else if (auto* alloca = dyn_cast<AllocaInst>(I))
            {
                std::string name = alloca->getName().str();

                std::cout << "ALLOCA removed -> " << name << "\n";

                
            }
        }
    }


    for (auto it = F.getEntryBlock().begin(); it != F.getEntryBlock().end(); )
    {
        Instruction* I = &*it++;

        if (auto* alloca = dyn_cast<AllocaInst>(I))
        {
            std::cout << "Removing ALLOCA -> " << alloca->getName().str() << "\n";
            alloca->eraseFromParent();
        }
    }
}