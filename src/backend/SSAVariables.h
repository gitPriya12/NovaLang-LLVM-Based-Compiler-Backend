#pragma once

#include "llvm/IR/BasicBlock.h"
#include <unordered_map>
#include <set>
#include <string>

extern std::unordered_map<
    llvm::BasicBlock*,
    std::set<std::string>> blockVariables;


// Strip SSA version numbers: y1 → y
inline std::string getBaseName(const std::string& name)
{
    std::string base;

    for (char c : name)
    {
        if (std::isdigit(c))
            break;
        base.push_back(c);
    }

    return base;
}