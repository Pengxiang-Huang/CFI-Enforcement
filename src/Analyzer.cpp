#include "Analyzer.h"

#include "llvm/IR/Instructions.h"
#include <memory>
using namespace llvm; /* put all functions as possible callee */
void IcallAnalyzer::constructIcallMap(
    std::vector<std::shared_ptr<Module>> modules) {

  std::set<Function *> funcSet;

  for (auto &m : modules) {
    for (auto &f : *m) {
      /*
       * check if function has body
       */
      if (f.isDeclaration()) {
        continue;
      }
      funcSet.insert(&f);
    }
  }

  for (auto &m : modules) {
    for (auto &f : *m) {
      for (auto &bb : f) {
        for (auto &inst : bb) {
          if (auto *callinst = dyn_cast<CallInst>(&inst)) {
            if (!callinst->getCalledFunction()) {
              errs() << "Found Icall: " << *callinst << " In " << f.getName()
                     << " In " << m->getName() << "\n";
              ICallMap[callinst] = funcSet;
            }
          }
        }
      }
    }
  }

  return;
}

std::set<Function *> &IcallAnalyzer::getPossibleCallees(CallInst *inst) {
  return ICallMap[inst];
}

void IcallAnalyzer::dumpIcallMap() {
  errs() << "dumping Icall Map: \n";
  errs() << "---------------------------\n";
  for (const auto &entry : ICallMap) {
    auto callInst = entry.first;
    const std::set<Function *> &callees = entry.second;

    Function *callerFunc = callInst->getFunction();
    if (!callerFunc) {
      errs() << "Error: CallInst has no parent function\n";
      continue;
    }

    Module *callerModule = callerFunc->getParent();
    if (!callerModule) {
      errs() << "Error: Function has no parent module\n";
      continue;
    }

    errs() << "Module: " << callerModule->getName() << "\n";
    errs() << "Function: " << callerFunc->getName() << "\n";
    errs() << "Instruction: " << *callInst << "\n";

    errs() << "Possible callees:\n";
    for (Function *callee : callees) {
      if (callee) {
        errs() << "  " << callee->getName() << "\n";
      } else {
        errs() << "  (Unknown callee)\n";
      }
    }
    errs() << "---------------------------\n";
  }
}
