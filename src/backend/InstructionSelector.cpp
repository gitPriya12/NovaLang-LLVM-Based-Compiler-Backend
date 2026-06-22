#include "InstructionSelector.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/CFG.h"

using namespace llvm;

static std::string getValueName(Value* v)
{
    if (!v) return "";

    if (v->hasName())
        return v->getName().str();

    if (auto* ci = dyn_cast<ConstantInt>(v))
        return std::to_string(ci->getSExtValue());

    return "tmp";
}

MachineFunction InstructionSelector::lower(Function& F)
{
    MachineFunction mf;
    mf.name = F.getName().str();

    for (auto& BB : F)
    {
        // ================= NON-TERMINATORS =================
        for (auto& I : BB)
        {
            if (I.isTerminator())
                break;

            if (auto* bin = dyn_cast<BinaryOperator>(&I))
            {
                mf.insts.emplace_back(
                    MOpcode::ADD,
                    bin->getName().str(),
                    getValueName(bin->getOperand(0)),
                    getValueName(bin->getOperand(1))
                );
            }

            else if (auto* cmp = dyn_cast<ICmpInst>(&I))
            {
                mf.insts.emplace_back(
                    MOpcode::CMP,
                    "cmp_tmp",
                    getValueName(cmp->getOperand(0)),
                    getValueName(cmp->getOperand(1))
                );
            }
        }

        // ================= TERMINATOR ONLY ONCE =================
        Instruction* term = BB.getTerminator();
        if (!term) continue;

        if (auto* br = dyn_cast<BranchInst>(term))
        {
            if (br->isConditional())
            {
                mf.insts.emplace_back(
                    MOpcode::BR,
                    "",
                    getValueName(br->getCondition()),
                    "",
                    br->getSuccessor(0)->getName().str() + "|" +
                    br->getSuccessor(1)->getName().str()
                );
            }
            else
            {
                mf.insts.emplace_back(
                    MOpcode::BR,
                    "",
                    "",
                    "",
                    br->getSuccessor(0)->getName().str()
                );
            }
        }

        else if (isa<ReturnInst>(term))
        {
            mf.insts.emplace_back(MOpcode::RET);
        }
    }

    return mf;
}