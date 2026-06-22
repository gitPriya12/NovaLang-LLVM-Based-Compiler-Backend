#pragma once

#include "machine/MachineFunction.h"
#include "llvm/IR/Function.h"

class InstructionSelector
{
public:
    MachineFunction lower(llvm::Function& F);
};