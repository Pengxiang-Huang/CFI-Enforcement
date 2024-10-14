# Set the source files
set (AnalyzerSourceCodes
    Config.h
    Config.cpp
    Utils.h
    Utils.cpp
    main.cpp
)

# Build object library
add_library(AnalyzerObj OBJECT ${AnalyzerSourceCodes})

# Build executable (cfi-analyzer) from object files
add_executable(cfi-analyzer $<TARGET_OBJECTS:AnalyzerObj>)

# Link the required LLVM libraries to cfi-analyzer
llvm_map_components_to_libnames(LLVM_LIBS support core irreader option)

target_link_libraries(cfi-analyzer
    ${LLVM_LIBS}
		LLVMOption
		LLVMSupport
		LLVMCore
    LLVMAnalysis
    LLVMAsmParser
    LLVMIRReader
)
