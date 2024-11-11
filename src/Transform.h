#include "Utils.h"
#include "Analyzer.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include <stdlib.h>
using namespace llvm;

class Optimizer {
    private:
        void transformTypeTests(LLVMContext &Context,std::vector<std::shared_ptr<Module>> modules);
        std::map<CallInst*, vector<std::string>> callsiteFunctionTypeMap;
	public:
		Optimizer() = default;
		void applyFunctionTransformation(Function *f, IcallAnalyzer * Analyzer,   std::vector<std::shared_ptr<Module>> modules);
        
};
