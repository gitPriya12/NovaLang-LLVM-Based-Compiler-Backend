#include "RegisterAllocator.h"

std::string RegisterAllocator::getReg(const std::string& v)
{
    if (v.empty())
        return "";

    if (map.count(v))
        return map[v];

    return map[v] = "R" + std::to_string(nextReg++);
}

void RegisterAllocator::allocate(MachineFunction& mf)
{
    map.clear();
    nextReg = 0;

    for (auto& inst : mf.insts)
    {
        inst.dst = getReg(inst.dst);
        inst.src1 = getReg(inst.src1);
        inst.src2 = getReg(inst.src2);
    }
}