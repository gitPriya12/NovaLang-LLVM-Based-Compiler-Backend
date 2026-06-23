# NovaLang-LLVM-Based-Compiler-Backend
Built a toy programming language compiler from scratch using C++ and LLVM.   Compiler Design LLVM C++ Intermediate Representation (IR) Static Single Assignment (SSA) Dominator Tree Control Flow Graph (CFG) Optimization Passes Register Allocation Instruction Selection Backend Engineering


# NovaLang Compiler (LLVM-Based Mini Compiler)

NovaLang is a custom programming language compiler built from scratch in C++ using LLVM infrastructure.  
It demonstrates a complete end-to-end compiler pipeline from source code to machine-level instructions.

---

## 🚀 Features

### Frontend
- Lexical Analysis (Tokenizer)
- Recursive Descent Parser
- Abstract Syntax Tree (AST)
- Semantic Analysis

### Intermediate Representation
- LLVM IR Generation
- Control Flow Graph (CFG) Construction
- Dominator Tree Analysis
- Immediate Dominator Computation
- Dominance Frontier Computation

### SSA & IR Transformation
- Static Single Assignment (SSA) Construction
- PHI Node Insertion
- SSA Renaming
- Mem2Reg-style Promotion

### Optimization Pipeline
- Constant Folding
- Dead Code Elimination (DCE)
- Copy Propagation
- Control Flow Graph Simplification

### Backend
- Instruction Selection (LLVM IR → Machine IR)
- Machine Instruction Generation
- Basic Register Allocation
- Machine Code Printing

---

## 🧠 Compiler Pipeline
Source Code
↓
Lexer
↓
Parser
↓
AST
↓
Semantic Analysis
↓
LLVM IR Generation
↓
CFG Construction
↓
Dominator Analysis
↓
SSA Construction
↓
PHI Insertion
↓
SSA Renaming
↓
Optimization Passes
↓
Instruction Selection
↓
Machine IR
↓
Register Allocation
↓
Final Machine Code

---


#Example Program
#Input:

let x = 3;

if (x < 5)
{
    let y = 1;
}
else
{
    let y = 2;
}

return y;

#Output:

===== MACHINE CODE =====
CMP R0, R1, R2
BR then|else
BR merge
BR merge
RET

#Build Instructions:

#Requirements:
CMake
LLVM (installed and configured)
C++17 compatible compiler

#Build Steps:
mkdir build
cd build

cmake --build .
#Run:
 .\Debug\NovaLang.exe


