#include "Transform.h"
#include "Utils.h"
#include "Config.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Mangler.h"
#include <stdlib.h>
#include <string>
using namespace llvm;







// @param Input: Instruction, read mismatch file that contains the source location, and to type
// @return: true if the location match the mismatch file, false otherwise
CallsiteLocation getSourceLocation(Instruction *I) {
    if (DILocation *Loc = I->getDebugLoc()) {
        unsigned Line = Loc->getLine();
        StringRef File = Loc->getFilename();
        StringRef Dir = Loc->getDirectory();
        errs() << "Source location: " << Dir << "/" << File << ":" << Line << "\n";
        return {File.str(), Line};
    }
    errs() << "No source location found for Instruction: " << *I << "\n";
    return {"error",0};
}


// Apply transformation to llvm.type.test functions to let it accept more types
void Optimizer::transformTypeTests(LLVMContext &Context,std::vector<std::shared_ptr<Module>> modules) {

  for (auto pair:callsiteFunctionTypeMap){
    CallInst *CI = pair.first;
    BranchInst *brInstruction = dyn_cast<BranchInst>(CI->getNextNode());
    if (!brInstruction) {
      errs() << "Error: llvm.type.test does not have a branch instruction\n";
      continue;
    }
    vector<string> functionTypes = pair.second;
    // CREATE llvm.type.test
    IRBuilder<> builder(CI);
    builder.SetInsertPoint(CI->getParent(), ++builder.GetInsertPoint());
    

    vector<Value*> typeTestResults;


    for(string functionType: functionTypes){
        // Create Random TYpe metadata
        MDString *MDString = MDString::get(Context, functionType);
        // CREATE  type test call
        Function *typeTestIntrinsic = Intrinsic::getDeclaration(
            CI->getParent()->getParent()->getParent(), Intrinsic::type_test);
        Value *typeTestResult = builder.CreateCall(
            typeTestIntrinsic,
            {CI->getArgOperand(0), MetadataAsValue::get(Context, MDString)});
        typeTestResults.push_back(typeTestResult);
    }

    Value* lastResult = CI;
    // Create an OR instruction , and change the result of the branch
    for(Value* typeTestResult: typeTestResults){
        lastResult = builder.CreateOr(lastResult, typeTestResult);
    }

    // Change the final branch instruction
    brInstruction->setCondition(lastResult);

//     Function *typeTestIntrinsic = Intrinsic::getDeclaration(
//         CI->getParent()->getParent()->getParent(), Intrinsic::type_test);

//     // Create Random TYpe metadata
//     string TypeStr = getTypeMetadata();
//     MDString *MDString = MDString::get(Context, TypeStr);
    
//     // CREATE  type test call
//     Value *typeTestResult = builder.CreateCall(
//         typeTestIntrinsic,
//         {CI->getArgOperand(0), MetadataAsValue::get(Context, MDString)});
//   }


//   for (auto CI : testedCallSiteSet) {


//     // Create an OR instruction , and change the result of the branch
//     // instruction
//     Value *orInstruction = builder.CreateOr(CI, typeTestResult);
//     Instruction *nextInst = CI->getNextNode()->getNextNode()->getNextNode();
//     if (BranchInst *br = dyn_cast<BranchInst>(nextInst)) {
//       br->setCondition(orInstruction);
//     }
  }
}


void Optimizer::applyFunctionTransformation(Function *f,
                                            IcallAnalyzer *analyzer,   std::vector<std::shared_ptr<Module>> modules) {

  errs() << "Performing Transformation  in Function: " << f->getName() << "\n";
  LLVMContext &Context = f->getContext();


  // read mismatch File
  std::map<CallsiteLocation, vector<std::string>> mismatchMap =readMismatchFile(mismatchFileName);
  std::map<CallsiteLocation, vector<std::string>> mismatchFunctionTypeMap = getMismatchFunctionTypes(modules, mismatchMap);




  for (auto &bb : *f) {
    for (auto &i : bb) {
      if (auto *CI = llvm::dyn_cast<llvm::CallInst>(&i)) {
        if (CI->getCalledFunction() &&
            CI->getCalledFunction()->getName() == "llvm.type.test") {

          // GET the corresponding icall for the llvm.type.test
          // Just found llvm.type.test share the same source location with the
          // icall
          CallsiteLocation CIlocation = getSourceLocation(CI);
          for (auto pair: mismatchFunctionTypeMap){
              CallsiteLocation location = pair.first;
              if (location.filename == CIlocation.filename && location.line == CIlocation.line){
                    callsiteFunctionTypeMap.insert({CI, pair.second});
              }
          }

          // PX's code
          auto *testedPtrs = CI->getArgOperand(0);
          errs() << "Found a testing function pointer: " << *testedPtrs << "\n";
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
              // What does operand in metadata node mean?
              // FIXME HERE
              if (MDNode) {
                for (unsigned i = 0; i < MDNode->getNumOperands(); ++i) {

                  Metadata *Op = MDNode->getOperand(i);
                  if (Op) // Op might be null in the program, don't know the
                          // reason yet
                    if (auto *Str = dyn_cast<MDString>(Op)) {
                      errs() << "    Operand " << i << ": " << Str->getString()
                             << "\n";
                    } else if (auto *IntVal =
                                   dyn_cast<ConstantAsMetadata>(Op)) {
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
          }
        }
      }
    }
  } 

  // Insertion of the or instructions
  transformTypeTests(Context,modules);
}






// -----------------  OLD CODE -----------------
/*
Find the icall that corresponds to llvm.type.test
            if(auto br=dyn_cast<llvm::BranchInst>(CI->getNextNode())){
              BasicBlock *trueBranch = br->getSuccessor(0);
              for(auto &i : *trueBranch){
                if(auto *CI = dyn_cast<llvm::CallInst>(&i)){
                  errs() << "The CallInst is: " << *CI << "\n";
                  if(getSourceLocation(CI)){
                    testedCallSiteSet.insert(CI);
                  }
                  break;
                    // Judge if the CI is what we want

                }
            }
              }

*/

// PREVIOUS IDEA: ADDING  MULTIPLE llvm.type.test checks, modify Branching
// instruction and adding new trap code blocks
/*
            // YT: insert another type.test in specific position
            //          CREATE FUNCTION TYPE:
            Type *VoidTy = Type::getVoidTy(Context);
            Type *VoidPtrTy = Type::getInt8PtrTy(
                Context); // `void*` is represented as `i8*` in LLVM
            FunctionType *FuncType =
                FunctionType::get(VoidTy, {VoidPtrTy}, false);

            // Value *TypeToBeChecked = FuncType;
            //  lvalue is auto *testedPtr = CI->getArgOperand(0);
            auto *testedPtr = CI->getArgOperand(0);
            // "_ZTSFvPvE" using mangler, here we just use a random string

            // YT: set a random metadata to be used in llvm.type.test
            // If you want to print a type, you have to do it in this way.

            std::vector<Metadata *> MetadataOperands;
            std::string TypeStr;
            raw_string_ostream RSO(TypeStr);
            FuncType->print(RSO);


            MetadataOperands.push_back(MDString::get(Context, TypeStr));
            // Create the MD_type metadata node
            MDNode *TypeMetadata = MDNode::get(Context, MetadataOperands);


            //: GET INSERTION POINT: insert next to the current instruction
            IRBuilder<> builder(CI);
            builder.SetInsertPoint(&bb, ++builder.GetInsertPoint());
            Function *typeTestIntrinsic =
                Intrinsic::getDeclaration(f->getParent(), Intrinsic::type_test);
            builder.CreateCall(
                typeTestIntrinsic,
                {testedPtr, MetadataAsValue::get(Context, TypeMetadata)});





            // // GET A NEW BASIC BLOCK: success
            // numOfAddedTests++;
            // string BBName = "Test" + to_string(numOfAddedTests);
            // BasicBlock *Test2BB = BasicBlock::Create(Context, BBName, f);
            // IRBuilder<> BuilderBB(Test2BB);
            // // NEW BB, TYPETEST
            // auto *bbTypeTest = BuilderBB.CreateCall(
            //     typeTestIntrinsic,
            //     {testedPtr, MetadataAsValue::get(Context, TypeMetadata)});
            // bbTypeTest->setMetadata("testType", TypeMetadata);





            // // GET BRANCHING INSTRUCTION : next instruction
            // auto *nextInst = CI->getNextNode()->getNextNode();
            // llvm::errs() << "YT: Next instruction: " << *nextInst << "\n";

            // if (auto *branchInst = dyn_cast<BranchInst>(nextInst)) {
            //   errs() << "Branching instruction content: " << *branchInst
            //          << "\n";
            //   if (branchInst->isConditional()) { // SHOULD BE
            // // Modify both successors of the current conditional branching
   operation

            //     BasicBlock *trueBranch = branchInst->getSuccessor(0);
            //     BasicBlock *falseBranch = branchInst->getSuccessor(1);
            //     branchInst->setSuccessor(0, falseBranch);
            //     branchInst->setSuccessor(1, Test2BB);
            //     errs() << "True branch: " << trueBranch->getName() << "\n";
            //     errs() << "False branch: " << falseBranch->getName() << "\n";
            //     BuilderBB.CreateCondBr(bbTypeTest, trueBranch, falseBranch);
            //   } else {
            //     BasicBlock *unconditionalBranch =
   branchInst->getSuccessor(0);
            //     errs() << "Unconditional branch: "
            //            << unconditionalBranch->getName() << "\n";
            //   }

            //   // CHANGE BRANCH CONTENT
            //}
    */