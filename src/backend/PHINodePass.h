#pragma once

#include <unordered_map>
#include <set>

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"

class PHINodePass
{
public:

    void run(
        llvm::Function* function);
};