#pragma once
#include "llvm/IR/Function.h"

class CopyPropagation
{
public:
    void run(llvm::Function& F);
};