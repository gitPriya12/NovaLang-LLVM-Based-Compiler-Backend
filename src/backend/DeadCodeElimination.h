#pragma once
#include "llvm/IR/Function.h"

class DeadCodeElimination
{
public:
    void run(llvm::Function& F);
};