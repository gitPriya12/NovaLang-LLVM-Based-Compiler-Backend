#pragma once

#include "llvm/IR/Module.h"

class OptimizationPipeline
{
public:
    void run(llvm::Module& module);
};