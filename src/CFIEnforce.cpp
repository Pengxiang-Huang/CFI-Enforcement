#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "llvm/Pass.h"

using namespace llvm;

namespace {
  struct CFIEnforcementPass : public ModulePass {
    static char ID;
    CFIEnforcementPass() : ModulePass(ID) {}

		bool doInitialization(Module &M) override {
			errs() << "do initialization!\n";
			return false;
		}

    bool runOnModule(Module &M) override {
			errs() << "run on module pass!\n";
      return false;
    }
  };
}

char CFIEnforcementPass::ID = 0;

// Register the pass with a name and description
static RegisterPass<CFIEnforcementPass> X("CFIEnforcement", "CFI Enforcement Pass", false, false);

// Automatically register the pass for -O3 and other optimization levels
static void registerCFIEnforcePass(const PassManagerBuilder &, legacy::PassManagerBase &PM) {
    PM.add(new CFIEnforcementPass());
}

// Register for different optimization levels
static RegisterStandardPasses RegisterCFIEnforcePass(
    PassManagerBuilder::EP_ModuleOptimizerEarly, registerCFIEnforcePass);
static RegisterStandardPasses RegisterCFIEnforcePass0(
    PassManagerBuilder::EP_EnabledOnOptLevel0, registerCFIEnforcePass);

