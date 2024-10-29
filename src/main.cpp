#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include <memory>

#include "Transform.h"
#include "Utils.h"

using namespace llvm;

// Command line parameters.
cl::list<std::string> InputFilenames(cl::Positional, cl::OneOrMore,
                                     cl::desc("<input bitcode files>"));

cl::opt<unsigned>
    VerboseLevel("verbose-level",
                 cl::desc("Print information at which verbose level"),
                 cl::init(0));

int main(int argc, char **argv) {

  // Print a stack trace if we signal out.
  sys::PrintStackTraceOnErrorSignal(argv[0]);
  PrettyStackTraceProgram X(argc, argv);

  llvm_shutdown_obj Y; // Call llvm_shutdown() on exit.

  cl::ParseCommandLineOptions(argc, argv, "global analysis\n");
  SMDiagnostic Err;

  // Loading modules
  OP << "Total " << InputFilenames.size() << " file(s)\n";

  std::vector<std::shared_ptr<Module>> modules;

  // LLVMContext LLVMCtx;
  for (unsigned i = 0; i < InputFilenames.size(); ++i) {
    LLVMContext *LLVMCtx = new LLVMContext();

    std::unique_ptr<Module> M = parseIRFile(InputFilenames[i], Err, *LLVMCtx);

    if (M == NULL) {
      OP << argv[0] << ": error loading file '" << InputFilenames[i] << "'\n";
      continue;
    }

    modules.push_back(std::move(M));
  }

  auto analyzer = new IcallAnalyzer();
  analyzer->constructIcallMap(modules);
  analyzer->dumpIcallMap();

  auto optimizer = new Optmizer();

  for (auto &m : modules) {
    for (auto &f : *m) {
      /*
       * check if function has body
       */
      if (f.isDeclaration()) {
        continue;
      }
      optimizer->applyFunctionTransformation(&f, analyzer);
    }
  }

  // YT print the changed IR
   std::error_code EC;
   const char* FIlename="/home/yantingchi/Desktop/static_analysis/tools/CFI-Enforcement/dispatch_changed.ll";
  raw_fd_ostream outFile(FIlename, EC, sys::fs::OF_None);
   for(auto module:modules){
    // MODIFY HERE
    module->print(outFile, nullptr);
   }
  delete analyzer;
  delete optimizer;
  return 0;
}
