#pragma once

#include <string>
#include <vector>
enum class MOpcode
{
    MOV,
    ADD,
    SUB,
    MUL,
    CMP,
    BR,
    RET
};

struct MachineInst
{
    MOpcode op;

    std::string dst;    // result (optional for CMP/BR)
    std::string src1;
    std::string src2;

    std::string label;  // branch target

    MachineInst(MOpcode op,
        std::string d = "",
        std::string s1 = "",
        std::string s2 = "",
        std::string l = "")
        : op(op), dst(d), src1(s1), src2(s2), label(l) {
    }
};