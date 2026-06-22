#include <iostream>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "semantic/SemanticAnalyzer.h"
#include "codegen/LLVMCodeGen.h"

#include "backend/CFGBuilder.h"
#include "backend/DominatorAnalysis.h"
#include "backend/IDominatorTree.h"
#include "backend/DominanceFrontier.h"

#include "backend/SSABuilder.h"
#include "backend/PHINodePass.h"
#include "backend/PHIInserter.h"
#include "backend/PHIBuilder.h"
#include "backend/SSARenamer.h"
#include "backend/Mem2Reg.h"
#include "backend/InstructionSelector.h"
#include "backend/MachinePrinter.h"
#include "backend/machine/RegisterAllocator.h"


int main()
{
    std::string source = R"(
let x = 3;
let y;

if (x < 5)
{
    y = 1;
}
else
{
    y = 2;
}

return y;
)";

    // ================= FRONTEND =================
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parseProgram();

    SemanticAnalyzer analyzer;
    analyzer.analyze(program.get());

    LLVMCodeGen codegen;
    codegen.generate(program.get());

    

    llvm::Module* module = codegen.getModule();
    llvm::Function* mainFunction = module->getFunction("main");

    if (!mainFunction)
    {
        std::cerr << "main not found\n";
        return 1;
    }

    // ================= ANALYSIS =================
    CFGBuilder cfg;
    for (auto& fn : *module)
        cfg.build(&fn);

    DominatorAnalysis dom;
    dom.compute(mainFunction);

    IDominatorTree idom;
    idom.compute(mainFunction);

    DominanceFrontier df;
    df.compute(mainFunction, idom);

    auto frontiers = df.getFrontiers();

    // ================= SSA =================
    SSABuilder ssa;
    ssa.build(*mainFunction, frontiers);

    Mem2Reg mem2reg;
    mem2reg.run(*mainFunction);



    // ================= PHI (ONLY ONE SYSTEM) =================
    PHIBuilder phiBuilder;
    phiBuilder.run(mainFunction, frontiers, ssa.getBlockDefs());

    // ================= SSA RENAMER =================
    SSARenamer renamer;
    renamer.run(mainFunction);

    codegen.printIR();

    // ================= MACHINE CODE =================
    InstructionSelector selector;
    MachineFunction mf = selector.lower(*mainFunction);

    // ================= REGISTER ALLOCATION =================
    RegisterAllocator ra;
    ra.allocate(mf);

    // ================= PRINT FINAL OUTPUT =================
    MachinePrinter::print(mf);

    return 0;
    return 0;
}