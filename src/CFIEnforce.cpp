#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
  struct CFIEnforce : public FunctionPass {
    static char ID; 

    CFIEnforce() : FunctionPass(ID) {}

    bool doInitialization (Module &M) override {
      errs() << "Hello LLVM World at \"doInitialization\"\n" ;
      return false;
    }

    bool runOnFunction (Function &F) override {
      errs() << "Hello LLVM World at \"runOnFunction\"\n" ;
      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      errs() << "Hello LLVM World at \"getAnalysisUsage\"\n" ;
      AU.setPreservesAll();
    }
  };
}

// Next there is code to register your pass to "opt"
char CFIEnforce::ID = 0;
static RegisterPass<CFIEnforce> X("CFIEnforce", "CFIEnforcement");

// Next there is code to register your pass to "clang"
static CFIEnforce * _PassMaker = NULL;
static RegisterStandardPasses _RegPass1(PassManagerBuilder::EP_OptimizerLast,
    [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
        if(!_PassMaker){ PM.add(_PassMaker = new CFIEnforce());}}); // ** for -Ox
static RegisterStandardPasses _RegPass2(PassManagerBuilder::EP_EnabledOnOptLevel0,
    [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
        if(!_PassMaker){ PM.add(_PassMaker = new CFIEnforce()); }}); // ** for -O0
