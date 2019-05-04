cmake_minimum_required(VERSION 3.3.0)

include(CMakeParseArguments)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

project(yosys)

# run makefile provided 
add_custom_target(
  yosys ALL 
  COMMAND make #-j32 
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Compile Yosys with given Makefile"
)


#add_custom_command(TARGET yosys COMMAND make)




