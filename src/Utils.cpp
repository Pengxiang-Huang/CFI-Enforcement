#include "Utils.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/IR/InlineAsm.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Operator.h>

using namespace llvm;

// @param Input: Instruction, read mismatch file that contains the source
// location, and to type
// @return: true if the location match the mismatch file, false otherwise
CallsiteLocation getSourceLocation(Instruction *I) {
  if (DILocation *Loc = I->getDebugLoc()) {
    unsigned Line = Loc->getLine();
    StringRef File = Loc->getFilename();
    StringRef Dir = Loc->getDirectory();
    errs() << "Source location: " << Dir << "/" << File << ":" << Line << "\n";
    return {File.str(), Line};
  }
  errs() << "No source location found for Instruction: " << *I << "\n";
  return {"error", 0};
}
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
std::string getBaseName(const std::string &filename) {
  size_t lastDot = filename.find_last_of('.');
  if (lastDot == std::string::npos) {
    return filename;
  }
  return filename.substr(0, lastDot);
}

void writeBitcodeToFile(const llvm::Module *Module,
                        const std::string &Filename) {
  std::error_code EC;
  llvm::raw_fd_ostream OS(Filename, EC);
  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message() << "\n";
    return;
  }
  llvm::WriteBitcodeToFile(*Module, OS);
  OS.flush();
}
