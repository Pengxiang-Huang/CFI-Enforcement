#include "Utils.h"
#include "Analyzer.h"

using namespace llvm;

class Optmizer {
	public:
		Optmizer() = default;
		void applyFunctionTransformation(Function *f, IcallAnalyzer * Analyzer);
};
