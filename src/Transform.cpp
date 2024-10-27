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

          /*
           * output all callees
           */
          auto funcset = analyzer->getPossibleCalleesInModule();

          for (auto f : funcset) {
            errs() << "output f name: " << f->getName() << "\n";
            SmallVector<std::pair<unsigned, MDNode *>, 4> MDs;
            (*f).getAllMetadata(MDs);
            // Print all metadata
            for (const auto &MD : MDs) {
              unsigned KindID = MD.first;
              MDNode *MDNode = MD.second;

              errs() << "  Metadata kind ID: " << KindID << "\n";

              // Print each operand in the metadata node
              if (MDNode) {
                for (unsigned i = 0; i < MDNode->getNumOperands(); ++i) {
                  Metadata *Op = MDNode->getOperand(i);
                  if (auto *Str = dyn_cast<MDString>(Op)) {
                    errs() << "    Operand " << i << ": " << Str->getString()
                           << "\n";
                  } else if (auto *IntVal = dyn_cast<ConstantAsMetadata>(Op)) {
                    // Handle integer values
                    if (auto *ConstInt =
                            dyn_cast<ConstantInt>(IntVal->getValue())) {
                      errs() << "    Operand " << i << ": "
                             << ConstInt->getZExtValue() << "\n";
                    }
                  } else {
                    errs() << "    Operand " << i
                           << ": (non-string, non-integer type)\n";
                  }
                }
              } else {
                errs() << "  No metadata available.\n";
              }
            }
            /*
             * TODO
             * tranformation
             * create call and or instructions
             */
          }
        }
      }
    }
  }
}
