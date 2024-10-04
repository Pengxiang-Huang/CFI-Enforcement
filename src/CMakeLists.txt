# Sources
file(GLOB CFIEnforceSrc "*.cpp")

# Name of the LLVM pass
set(CFIEnforceName "CFIEnforcement")

# Configure LLVM
find_package(LLVM REQUIRED CONFIG)

# Output directories
set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/)
set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/)

include_directories(${LLVM_INCLUDE_DIRS})

set_source_files_properties(${CFIEnforceSrc} PROPERTIES COMPILE_FLAGS "-std=c++20")

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
include(AddLLVM)

message(STATUS "LLVM_DIR IS ${LLVM_CMAKE_DIR}.")

include_directories(${LLVM_INCLUDE_DIRS})

# Declare the LLVM pass to compile
add_llvm_library(${CFIEnforceName} MODULE ${CFIEnforceSrc})
