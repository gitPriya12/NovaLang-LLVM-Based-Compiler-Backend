#pragma once

#include "MachineInst.h"
#include <vector>
#include <string>

struct MachineFunction
{
    std::string name;
    std::vector<MachineInst> insts;

    void add(const MachineInst& inst)
    {
        insts.push_back(inst);
    }
};