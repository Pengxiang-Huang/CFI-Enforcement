#include "Transform.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"

using namespace llvm;
int INSERT = 1;

void Optmizer::applyFunctionTransformation(Function *f,
                                           IcallAnalyzer *analyzer) {
  errs() << "Performing Transformation  in Function: " << f->getName() << "\n";

  for (auto &bb : *f) {
    for (auto &i : bb) {
      if (auto *CI = llvm::dyn_cast<llvm::CallInst>(&i)) {
        if (CI->getCalledFunction() &&
            CI->getCalledFunction()->getName() == "llvm.type.test") {
          if (INSERT) {
            // YT: insert another type.test in specific position
            //          CREATE FUNCTION TYPE:
            LLVMContext &Context = f->getContext();
            Type *VoidTy = Type::getVoidTy(Context);
            Type *VoidPtrTy = Type::getInt8PtrTy(
                Context); // `void*` is represented as `i8*` in LLVM
            FunctionType *FuncType =
                FunctionType::get(VoidTy, {VoidPtrTy}, false);


            // Value *TypeToBeChecked = FuncType;
            //  lvalue is auto *testedPtr = CI->getArgOperand(0);
            auto *testedPtr = CI->getArgOperand(0);
            // rvalue: seems to be metadata, we can generate  identifier like
            // "_ZTSFvPvE" using mangler, here we just use a random string

            // YT: set a random metadata to be used in llvm.type.test
            std::vector<Metadata *> MetadataOperands;
            std::string TypeStr;
            raw_string_ostream RSO(TypeStr);
            FuncType->print(RSO);
            MetadataOperands.push_back(MDString::get(Context, TypeStr));
            // Create the MD_type metadata node
            MDNode *TypeMetadata = MDNode::get(Context, MetadataOperands);

            // YT: insert llvm.type.test success!
            //: GET INSERTION POINT: insert next to the current instruction
            IRBuilder<> builder(CI);
            builder.SetInsertPoint(&bb, ++builder.GetInsertPoint());
            Function *typeTestIntrinsic =
                Intrinsic::getDeclaration(f->getParent(), Intrinsic::type_test);
            builder.CreateCall(
                typeTestIntrinsic,
                {testedPtr, MetadataAsValue::get(Context, TypeMetadata)});
            INSERT = 0;
          }

          // branching instructions



          // PX's code
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
