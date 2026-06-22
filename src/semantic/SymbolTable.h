#pragma once

#include <unordered_map>
#include <string>

#include "Symbol.h"

class SymbolTable
{
private:

    std::unordered_map<
        std::string,
        Symbol
    > symbols;

public:

    bool insert(
        const std::string& name,
        Type type);

    bool exists(
        const std::string& name);

    Symbol* lookup(
        const std::string& name);
};