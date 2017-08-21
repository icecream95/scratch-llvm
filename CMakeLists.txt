cmake_minimum_required (VERSION 2.6)
project (scratch-llvm)
message(STATUS "Configuring scratch-llvm...")

set (SLLVM_VERSION_MAJOR 0)
set (SLLVM_VERSION_MINOR 0)
set (SLLVM_VERSION_UPDATE 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/include/scratch-llvm/scratch-llvm.hpp.in"
  "${PROJECT_BINARY_DIR}/src/include/scratch-llvm/scratch-llvm.hpp"
  )

include_directories("${PROJECT_BINARY_DIR}/src/include")
include_directories("${PROJECT_SOURCE_DIR}/src/include")

add_executable(llscratch src/llscratch.cpp)

