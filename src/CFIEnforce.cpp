#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
    struct CFIEnforcement : public ModulePass {
        static char ID;
        CFIEnforcement() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            errs() << "Running on module: " << M.getName() << "\n";
            for (Function &F : M) {
                errs() << "Function: " << F.getName() << "\n";
            }
            return false;  // If no modification to the module is made, return false
        }
    };
}

char CFIEnforcement::ID = 0;
static RegisterPass<CFIEnforcement> X("CFIEnforcement", "CFI Enforcement Pass", false, false);

// Automatically register the pass for -O3 and other optimization levels
static void registerCFIEnforcePass(const PassManagerBuilder &, legacy::PassManagerBase &PM) {
    PM.add(new CFIEnforcement());
}

// Register for different optimization levels
static RegisterStandardPasses RegisterCFIEnforcePass(
    PassManagerBuilder::EP_ModuleOptimizerEarly, registerCFIEnforcePass);
static RegisterStandardPasses RegisterCFIEnforcePass0(
    PassManagerBuilder::EP_EnabledOnOptLevel0, registerCFIEnforcePass);

