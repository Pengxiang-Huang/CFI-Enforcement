#include "Transform.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include <map>
#include <vector>

using namespace llvm;

void Optmizer::applyFunctionTransformation(Function *f,
                                           IcallAnalyzer *analyzer) {
  errs() << "Performing Transformation  in Function: " << f->getName() << "\n";

  std::map<Instruction *, std::vector<Value *>> tomodify;

  for (auto &bb : *f) {
    for (auto &i : bb) {
      if (auto *CI = llvm::dyn_cast<CallInst>(&i)) {

        /*
         * Perform Transformation after the llvm.type.test
         */
        if (CI->getCalledFunction() &&
            CI->getCalledFunction()->getName() == "llvm.type.test") {

          errs() << "Found llvm testing pointer: " << *CI << "\n";
          auto *testedPtr = CI->getArgOperand(0);

          std::vector<Value *> toadd;
          auto funcset = analyzer->getPossibleCalleesInModule();

          IRBuilder<> builder(CI);

          /*
           * create call and or instruction
           */
          for (auto func : funcset) {
            errs() << "adding allowed f name: " << func->getName() << "\n";
            SmallVector<std::pair<unsigned, MDNode *>, 4> MDs;
            (*func).getAllMetadata(MDs);
            // Print all metadata
            for (const auto &MD : MDs) {
              MDNode *MDNode = MD.second;

              // Print each operand in the metadata node
              if (MDNode) {
                for (unsigned i = 0; i < MDNode->getNumOperands(); ++i) {

                  Metadata *Op = MDNode->getOperand(i);
                  if (auto *Str = dyn_cast<MDString>(Op)) {
                    /*
                     * check the string if it starts with _ZTS mangled name
                     */
                    auto str = Str->getString();
                    if (str.startswith("_ZTS")) {
                      /*
                       * create the call instruction
                       */
                      Function *typeTestIntrinsic = Intrinsic::getDeclaration(
                          f->getParent(), Intrinsic::type_test);
                      auto typeTestCall = builder.CreateCall(
                          typeTestIntrinsic,
                          {testedPtr,
                           MetadataAsValue::get(f->getContext(), Op)});
                      errs() << *typeTestCall << "\n";
                      toadd.push_back(typeTestCall);
                    }
                  }
                }
              } else {
                errs() << "  No metadata available.\n";
              }
            }
          }

          tomodify[CI] = toadd;
          errs() << "-------------\n";
        }
      }
    }
  }

  // for (auto &pair : tomodify){
  // 	Instruction * insertPoint = pair.first;
  // 	auto &toadd = pair.second;
  // 	IRBuilder<> builder(insertPoint);
  // 	builder.SetInsertPoint(insertPoint->getParent(),
  // ++insertPoint->getIterator()); 	for (auto v : toadd){ 		errs() << *v << "\n";
  // 		builder.Insert(v);
  // 		builder.SetInsertPoint(insertPoint->getParent(),
  // ++insertPoint->getIterator());
  // 	}
  // }
}
