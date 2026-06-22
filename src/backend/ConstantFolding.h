#pragma once
#include "llvm/IR/Function.h"

class ConstantFolding
{
public:
    void run(llvm::Function& F);
};