#include "PHIBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include <iostream>
#include <set>

using namespace llvm;

#include <cctype>

static std::string getBaseName(
    const std::string& name)
{
    std::string base;

    for (char c : name)
    {
        if (std::isdigit(
            static_cast<unsigned char>(c)))
        {
            break;
        }

        base.push_back(c);
    }

    return base;
}

void PHIBuilder::run(
    Function* function,
    const std::unordered_map<BasicBlock*, std::set<BasicBlock*>>& df,
    const std::unordered_map<
    BasicBlock*,
    std::unordered_map<std::string, Value*>>&blockDefs)
{
    std::cout << "\n===== PHI INSERTION =====\n";

    std::set<std::pair<BasicBlock*, std::string>> inserted;

    for (auto& bb : *function)
    {
        auto it = df.find(&bb);
        if (it == df.end())
            continue;

        for (auto* frontier : it->second)
        {
            std::cout
                << "\nChecking frontier: "
                << frontier->getName().str()
                << "\n";

            std::set<std::string> vars;

            // collect variables
            for (const auto& entry : blockDefs)
            {
                const auto& defs = entry.second;

                for (const auto& d : defs)
                {
                    vars.insert(
                        getBaseName(d.first));
                }
            }

            for (const std::string& var : vars)
            {
                auto countDefs = [&](const std::string& v)
                    {
                        int count = 0;
                        for (auto& [bb, defs] : blockDefs)
                        {
                            for (const auto& def : defs)
                            {
                                if (getBaseName(def.first) == v)
                                {
                                    count++;
                                    break;
                                }
                            }
                        }
                        return count;
                    };

                if (countDefs(var) < 2)
                    continue;

                if (inserted.count({ frontier, var }))
                    continue;

                inserted.insert({ frontier, var });

                IRBuilder<> builder(frontier);

                builder.SetInsertPoint(
                    &*frontier->getFirstInsertionPt());

                PHINode* phi =
                    builder.CreatePHI(
                        Type::getInt32Ty(function->getContext()),
                        0,
                        var);

                std::cout
                    << "PHI count in block: "
                    << frontier->size()
                    << "\n";

                for (auto& [pred, defs] : blockDefs)
                {
                    for (const auto& def : defs)
                    {
                        if (getBaseName(def.first) == var)
                        {
                            phi->addIncoming(
                                def.second,
                                pred);

                            break;
                        }
                    }
                }

                std::cout << "PHI created for " << var
                    << " in " << frontier->getName().str()
                    << "\n";


                std::cout << "PHI count in block: "
                    << frontier->size()
                    << "\n";

                std::cout << "Incoming count: " << phi->getNumIncomingValues() << "\n";
            }
        }
    }
}