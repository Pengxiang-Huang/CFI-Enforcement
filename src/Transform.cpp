#include "Transform.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"

using namespace llvm;

void Optmizer::applyFunctionTransformation(Function *f,
                                           IcallAnalyzer *analyzer) {
  errs() << "Performing Transformation  in Function: " << f->getName() << "\n";

  for (auto &bb : *f) {
    for (auto &i : bb) {
      if (auto *CI = llvm::dyn_cast<llvm::CallInst>(&i)) {
        if (CI->getCalledFunction() &&
            CI->getCalledFunction()->getName() == "llvm.type.test") {
          auto *testedPtr = CI->getArgOperand(0);
          errs() << "Found a testing function pointer: " << *testedPtr << "\n";
          if (auto *MetaOp = dyn_cast<MetadataAsValue>(CI->getOperand(1))) {
            if (auto *MD = dyn_cast<MDString>(MetaOp->getMetadata())) {
              llvm::outs() << "Found metadata: " << MD->getString() << "\n";
            }
          }
          // auto type = testedPtr->getType();
          // auto typeName = type->getPointerElementType()->getStructName();
          // auto newTyMd = MDString::get(CI->getContext(), typeName);
          // IRBuilder<> builder(CI->getNextNode());
          // // Create the second llvm.type.test call with the new metadata
          //         MetadataAsValue *newTypeMeta =
          //         MetadataAsValue::get(CI->getContext(), newTyMd); Value
          //         *secondTypeTest =
          //         builder.CreateCall(CI->getCalledFunction(), {testedPtr,
          //         newTypeMeta});
          //
          //         // OR the results of the two type tests
          //         Value *orResult = builder.CreateOr(CI, secondTypeTest,
          //         "match_any");
          // // Update the branch instruction to use the OR result
          //         for (auto *user : CI->users()) {
          //             if (auto *brInst = dyn_cast<BranchInst>(user)) {
          //                 // If the branch instruction is conditional
          //                 if (brInst->isConditional()) {
          //                     brInst->setCondition(orResult);
          //                 }
          //             }
          //         }
          // errs() << "Inserted the second type test and update the branch\n";
        }
      }
    }
  }
}
