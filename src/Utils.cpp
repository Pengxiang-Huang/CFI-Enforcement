#include "Utils.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Metadata.h"
#include "llvm/Support/Error.h"
#include <llvm/IR/InlineAsm.h>

#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Operator.h>

using namespace llvm;

std::string getTypeMetadata(Function *f) {

  SmallVector<MDNode *, 2> Types;
  f->getMetadata(LLVMContext::MD_type, Types);
  for (auto *Type : Types) {
    errs() << "Type for : " << f->getName() << " ";
    Type->print(llvm::outs());
    llvm::outs() << "\n";
  }
  
  // The second one in SmallVector<MDNode *, 2> Types is "generalized type"
  // We use the first one
  // Type: <0x5c8f58dbdfd0> = !{i64 0, !"_ZTSFvPcE"}
  // Type: <0x5c8f58dbb360> = !{i64 0, !"_ZTSFvPvE.generalized"}
  for (auto Type : Types) {

    Metadata *MD = Type->getOperand(1);
    if (auto *MDStr = dyn_cast<MDString>(MD)) {
        if (MDStr->getString().str().find("generalized") != std::string::npos) {
            continue;
        }
      return MDStr->getString().str();
    }
  }

    errs() << "Error: Metadata is not a string.\n";
    return "";
  
}

std::set<string> getTypes(set<Function *> &functions) {
  std::set<string> types;
  for (Function *f : functions) {
    types.insert(getTypeMetadata(f));
  }
  return types;
}
