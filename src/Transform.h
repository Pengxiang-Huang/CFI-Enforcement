#include "Utils.h"
#include "Analyzer.h"

using namespace llvm;

class Optimizer {
	public:
		Optimizer() = default;
		void applyFunctionTransformation(Function *f, IcallAnalyzer * Analyzer);
	private:
			void transformTypeTests(LLVMContext &Context);
			std::map<CallInst*, std::set<Function*>> callsiteFunctionTypeMap;
};
