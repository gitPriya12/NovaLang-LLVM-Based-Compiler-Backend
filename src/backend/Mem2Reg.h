#pragma once

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"

#include <unordered_map>
#include <string>

class Mem2Reg
{
public:
    void run(llvm::Function& F);

private:
    std::unordered_map<std::string, llvm::Value*> currentValue;
};