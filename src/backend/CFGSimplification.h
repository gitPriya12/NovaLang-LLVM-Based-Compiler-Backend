#pragma once
#include "llvm/IR/Function.h"

class CFGSimplification
{
public:
    void run(llvm::Function& F);
};