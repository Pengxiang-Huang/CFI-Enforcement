#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"

#include <map>
#include <memory>
#include <set>
#include <vector>
using namespace llvm;


class IcallAnalyzer{
	public:
		IcallAnalyzer() = default;
		void constructIcallMap(std::vector<std::shared_ptr<Module>> modules); 
		std::set<Function*> & getPossibleCallees(CallInst* inst);
		std::set<Function*> & getPossibleCalleesInModule();
		void dumpIcallMap();
        std::map<CallInst*, std::set<Function*> > getICallMap();
        
	private:
		std::map<CallInst*, std::set<Function*> > ICallMap ;
};


