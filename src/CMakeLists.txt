# Set the source files
set (AnalyzerSourceCodes
    Config.h
    Config.cpp
    Utils.h
    Utils.cpp
    main.cpp
		Analyzer.h
		Analyzer.cpp
		Transform.h
		Transform.cpp
)

# Build object library
add_library(AnalyzerObj OBJECT ${AnalyzerSourceCodes})

# Build executable (cfi-analyzer) from object files
add_executable(cfi-enforce $<TARGET_OBJECTS:AnalyzerObj>)

# Link the required LLVM libraries to cfi-analyzer
llvm_map_components_to_libnames(LLVM_LIBS support core irreader option bitwriter analysis asmparser)

target_link_libraries(cfi-enforce
    ${LLVM_LIBS}
		LLVMOption
		LLVMSupport
		LLVMCore
		LLVMAnalysis
		LLVMAsmParser
		LLVMIRReader
)
