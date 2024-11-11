#ifndef CONFIG_H
#define CONFIG_H


#include "Utils.h"

using namespace llvm;

//
// Configurations
//

const string ROOT_PATH="/home/yantingchi/Desktop/static_analysis/tools/CFI-Enforcement/";





const string mismatchFileName = ROOT_PATH+"demo/icall/mismatch.list";
const string outputIRFName = ROOT_PATH+"demo/icall/output.ll";
#define DEBUG

#endif
