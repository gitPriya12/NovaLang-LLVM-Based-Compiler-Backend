#include "SSARenamer.h"
#include "llvm/IR/CFG.h"
#include <iostream>

using namespace llvm;

static std::string getBaseName(const std::string& name)
{
    std::string base;

    for (char c : name)
    {
        if (isdigit(c)) break;
        base.push_back(c);
    }

    return base;
}

void SSARenamer::run(Function* function)
{
    std::cout << "\n===== SSA RENAME =====\n";

    visited.clear();
    versionCounter.clear();
    versionStack.clear();
    currentValue.clear();

    renameBlock(&function->getEntryBlock());
}

void SSARenamer::renameBlock(BasicBlock* block)
{
    if (visited.count(block))
        return;

    visited.insert(block);

    std::cout << "Visiting " << block->getName().str() << "\n";

    // ================= PHI HANDLING =================
    for (auto& inst : *block)
    {
        if (auto* phi = dyn_cast<PHINode>(&inst))
        {
            std::string name = getBaseName(phi->getName().str());

            currentValue[name] = phi;
            versionStack[name].push_back(phi);

            std::cout << "Found PHI " << phi->getName().str() << "\n";
        }
    }

    // ================= STORE / LOAD =================
    for (auto it = block->begin(); it != block->end(); )
    {
        Instruction* inst = &*it++;

        // STORE
        if (auto* store = dyn_cast<StoreInst>(inst))
        {
            Value* ptr = store->getPointerOperand();
            Value* val = store->getValueOperand();

            if (!ptr->hasName())
                continue;

            std::string name = getBaseName(ptr->getName().str());

            currentValue[name] = val;

            std::cout << "STORE SSA -> " << name << "\n";

            store->eraseFromParent();
        }

        // LOAD
        else if (auto* load = dyn_cast<LoadInst>(inst))
        {
            Value* ptr = load->getPointerOperand();

            if (!ptr->hasName())
                continue;

            std::string name = getBaseName(ptr->getName().str());

            if (currentValue.count(name))
            {
                load->replaceAllUsesWith(currentValue[name]);
                std::cout << "LOAD SSA -> " << name << "\n";
            }

            load->eraseFromParent();
        }
    }

    // ================= DFS =================
    for (BasicBlock* succ : successors(block))
    {
        if (!visited.count(succ))
            renameBlock(succ);
    }

    // ================= BACKTRACK =================
    for (auto& [name, stack] : versionStack)
    {
        if (!stack.empty())
            stack.pop_back();
    }
}