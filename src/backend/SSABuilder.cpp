#include "SSABuilder.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include "SSAVariables.h"
#include <iostream>
#include <cctype>

using namespace llvm;



// ================= HELPERS =================

void SSABuilder::pushVersion(const std::string& name, Value* v)
{
    versionStack[name].push_back(v);
}

Value* SSABuilder::getCurrentVersion(const std::string& name)
{
    auto it = versionStack.find(name);
    if (it == versionStack.end() || it->second.empty())
        return nullptr;

    return it->second.back();
}

Value* SSABuilder::createNewVersion(const std::string& name, Value* v)
{
    versionCounter[name]++;
    pushVersion(name, v);
    return v;
}

// ================= MAIN SSA =================

void SSABuilder::build(
    Function& function,
    const std::unordered_map<BasicBlock*, std::set<BasicBlock*>>& df)
{
    std::cout << "\n===== SSA BUILDER =====\n";

    versionStack.clear();
    versionCounter.clear();

    for (auto& bb : function)
    {
        for (auto& inst : bb)
        {
            if (auto* store = dyn_cast<StoreInst>(&inst))
            {
                Value* ptr = store->getPointerOperand();
                Value* val = store->getValueOperand();

                if (!ptr || !ptr->hasName())
                    continue;

                std::string name = getBaseName(ptr->getName().str());

                int v = versionCounter[name]++;
                std::string newName = name + std::to_string(v);

                blockDefs[&bb][newName] = val;
            }

            else if (auto* load = dyn_cast<LoadInst>(&inst))
            {
                Value* ptr = load->getPointerOperand();

                if (!ptr || !ptr->hasName())
                    continue;

                std::string name = getBaseName(ptr->getName().str());

                Value* current = getCurrentVersion(name);

                if (current)
                    load->replaceAllUsesWith(current);
            }
        }
    }

    std::cout << "\n===== SSA STACK =====\n";

    for (auto& [bb, defs] : blockDefs)
    {
        std::cout << bb->getName().str() << ":\n";

        for (auto& [name, val] : defs)
        {
            std::cout << "  " << name << "\n";
        }
    }

    for (auto& it : versionStack)
    {
        std::cout << it.first << " : ";
        for (auto* v : it.second)
            std::cout << (v->hasName() ? v->getName().str() : "<tmp>") << " ";
        std::cout << "\n";
    }
}

