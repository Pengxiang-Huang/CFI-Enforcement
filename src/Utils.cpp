#include "Utils.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/IR/InlineAsm.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Operator.h>

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
