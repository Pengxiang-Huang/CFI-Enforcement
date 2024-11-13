#include "Utils.h"
#include "Analyzer.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include <stdlib.h>
using namespace llvm;

class Optimizer {
    private:
        void transformTypeTests(LLVMContext &Context);
        std::map<CallInst*, std::set<Function*>> callsiteFunctionTypeMap;
	public:
		Optimizer() = default;
		void applyFunctionTransformation(Function *f, IcallAnalyzer * Analyzer);
        
};
