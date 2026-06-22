#include "MachinePrinter.h"
#include <iostream>

void MachinePrinter::print(const MachineFunction& mf)
{
    std::cout << "\n===== MACHINE CODE =====\n";

    for (auto& I : mf.insts)
    {
        switch (I.op)
        {
        case MOpcode::MOV:
            std::cout << "MOV " << I.dst << ", " << I.src1 << "\n";
            break;

        case MOpcode::ADD:
            std::cout << "ADD " << I.dst << ", " << I.src1 << ", " << I.src2 << "\n";
            break;

        case MOpcode::CMP:
            std::cout << "CMP " << I.dst << ", " << I.src1 << ", " << I.src2 << "\n";
            break;

        case MOpcode::BR:
            std::cout << "BR " << I.label << "\n";
            break;

        case MOpcode::RET:
            std::cout << "RET\n";
            break;
        }
    }
}