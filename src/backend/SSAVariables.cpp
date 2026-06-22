#include "SSAVariables.h"

std::unordered_map<
    llvm::BasicBlock*,
    std::set<std::string>> blockVariables;