cmake_minimum_required(VERSION 3.20)

project(NovaLang)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    NovaLang

    src/main.cpp

    src/lexer/Lexer.cpp

    src/parser/Parser.cpp
  
    src/semantic/SymbolTable.cpp
    
    src/semantic/SemanticAnalyzer.cpp

    src/codegen/LLVMCodeGen.cpp

    src/backend/CFGBuilder.cpp

    src/backend/DominatorAnalysis.cpp

    src/backend/IDominatorTree.cpp

    src/backend/DominatorTree.cpp

    src/backend/DominanceFrontier.cpp

    src/backend/SSAVariables.cpp

    src/backend/PHINodePass.cpp

    src/backend/SSABuilder.cpp

    src/backend/PHIInserter.cpp
    src/backend/PHIBuilder.cpp

    src/backend/OptimizationPipeline.cpp

    src/backend/ConstantFolding.cpp
    src/backend/DeadCodeElimination.cpp
    src/backend/CopyPropagation.cpp
    src/backend/CFGSimplification.cpp
  
    src/backend/SSARenamer.cpp
    src/backend/Mem2Reg.cpp

    src/backend/InstructionSelector.cpp
    src/backend/MachinePrinter.cpp
    src/backend/machine/RegisterAllocator.cpp

    
)

set(LLVM_DIR
"C:/Users/PriyaPathak/source/repos/llvm-project/build/lib/cmake/llvm")

find_package(LLVM REQUIRED CONFIG)



message(STATUS "LLVM Found")
message(STATUS "LLVM Version: ${LLVM_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(
    LLVM_LIBS
    Core
    Support
)

target_link_libraries(
    NovaLang
    LLVMCore
    LLVMSupport
    LLVMIRReader

    LLVMPasses
    LLVMAnalysis
    LLVMTransformUtils
    LLVMInstCombine
    LLVMScalarOpts
    ${LLVM_LIBS}
)