#pragma once
#include "MachineInst.h"
#include "MachineFunction.h"
#include <unordered_map>
#include <string>

class RegisterAllocator
{
public:
    void allocate(MachineFunction& mf);   // MUST match EXACTLY

private:
    int nextReg = 0;
    std::unordered_map<std::string, std::string> map;

    std::string getReg(const std::string& v);
};