#include "Utils.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Metadata.h"
#include "llvm/Support/Error.h"
#include <llvm/IR/InlineAsm.h>

#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Operator.h>

using namespace llvm;
std::map<CallsiteLocation, vector<std::string>> readMismatchFile(std::string filename){
    std::map<CallsiteLocation, vector<std::string>> mismatchMap;
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        errs() << "ERROR opening inputFile: " << filename << "\n";
        return mismatchMap;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string filename;
        unsigned line;
        std::string functionName;
        if(!(iss >> filename >> line >> functionName)){
            errs() << "Error reading line: " << line << "\n";
            continue;
        }
        CallsiteLocation location = {filename, line};
        mismatchMap[location].push_back(functionName);
    }
    inputFile.close();
    return mismatchMap;
}

std::string getTypeMetadata(Function* f){
    
    SmallVector<MDNode *, 2> Types;
    f->getMetadata(LLVMContext::MD_type,Types);
    for (auto *Type : Types) {
        errs() << "Type for : " << f->getName() << " "; 
      Type->print(llvm::outs());
      llvm::outs() << "\n";
    }
    // The second one in SmallVector<MDNode *, 2> Types is "generalized type"
    // We use the first one
    //Type: <0x5c8f58dbdfd0> = !{i64 0, !"_ZTSFvPcE"}
    // Type: <0x5c8f58dbb360> = !{i64 0, !"_ZTSFvPvE.generalized"}
    Metadata* MD=Types[1]->getOperand(1);
    if  (auto *MDStr = dyn_cast<MDString>(MD)){
        return MDStr->getString().str();
    } else {
        errs() << "Error: Metadata is not a string.\n";
        return "";
    }

    

}

// This procedure could be accelerated using a map to record the {functionName, functionMangledType} match
std::map<CallsiteLocation, vector<std::string>> 
getMismatchFunctionTypes(std::vector<std::shared_ptr<Module>> modules,
                         std::map<CallsiteLocation, vector<std::string>> &mismatchMap) {

  std::map<CallsiteLocation, vector<std::string>> mismatchFunctionTypesMap;
  
  // initialize
  for (auto item: mismatchMap){
      mismatchFunctionTypesMap[item.first] = {};
  }


  for (auto M : modules) {
    for (GlobalObject &GO : M->global_objects()) {
      if (Function *F = dyn_cast<Function>(&GO)) {
        for(auto item: mismatchMap){
            for (auto functionName: item.second){
                if (F->getName() == functionName){
                    CallsiteLocation location = item.first;
                    mismatchFunctionTypesMap[location].push_back(getTypeMetadata(F));
                }
            }
        
      }
    }
  }
}
 return mismatchFunctionTypesMap;
}