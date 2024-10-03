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

set(NO_RTTI "-fno-rtti")
add_definitions(${NO_RTTI})

# Declare the shared library without distribution-specific checks
add_library(${CFIEnforceName} MODULE ${CFIEnforceSrc})

target_link_libraries(${CFIEnforceName}
  PRIVATE
  LLVMCore
  LLVMSupport
  LLVMipo
  LLVMIRReader 
)

# Export the pass as a loadable module for 'opt'
llvm_map_components_to_libnames(LLVM_LIBS core support ipo irreader)

# Install the shared library
install(TARGETS ${CFIEnforceName} LIBRARY DESTINATION lib)
