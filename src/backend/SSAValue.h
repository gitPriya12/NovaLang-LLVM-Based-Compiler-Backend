#pragma once
#include <string>

struct SSAValue
{
    std::string name;
    int version;

    SSAValue() : name(""), version(0) {}
    SSAValue(std::string n, int v) : name(n), version(v) {}

    std::string str() const
    {
        return name + std::to_string(version);
    }
};