#include "SymbolTable.h"

bool SymbolTable::insert(
    const std::string& name,
    Type type)
{
    if (exists(name))
    {
        return false;
    }

    symbols[name] =
    {
        name,
        type
    };

    return true;
}

bool SymbolTable::exists(
    const std::string& name)
{
    return symbols.find(name)
        != symbols.end();
}

Symbol* SymbolTable::lookup(
    const std::string& name)
{
    auto it = symbols.find(name);

    if (it == symbols.end())
    {
        return nullptr;
    }

    return &it->second;
}