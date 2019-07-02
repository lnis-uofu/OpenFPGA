cmake_minimum_required(VERSION 3.3.0)

include(CMakeParseArguments)

project(yosys)

# Version number 
set(YOSYS_VERSION_MAJOR 0.7)
set(YOSYS_VERSION_MINOR 0)
set(YOSYS_VERSION_PATCH 0)

# Options to enable/disable dependencies
option(YOSYS_ENABLE_TCL, "Enable TCL parser integrated in yosys" ON)
option(YOSYS_ENABLE_ABC, "Enable ABC library integrated in yosys" ON)
option(YOSYS_ENABLE_PLUGINS, "Enable plug-in in yosys" ON)
option(YOSYS_ENABLE_READLINE, "Enable readline library in yosys" ON)
option(YOSYS_ENABLE_VERIFIC, "Enable verification library in yosys" OFF)
option(YOSYS_ENABLE_COVER, "Enable coverage test in yosys" ON)
option(YOSYS_ENABLE_LIBYOSYS, "Enable static library compiled yosys" OFF)
option(YOSYS_ENABLE_GPROF, "Enable profiling in compiled yosys" OFF)
option(YOSYS_ENABLE_NDEBUG, "Enable non-debugging feature in compiled yosys" OFF)

#
## Search and link dependent packages
## We need readline to compile 
if (YOSYS_ENABLE_READLINE)
  find_package(Readline REQUIRED)
endif()
#
#########################
##                      #
## Compiler Flags Setup #
##                      #
#########################
#
## Compiler flag configuration checks
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
#
## Required Compiler Standard 
#set(CMAKE_CXX_STANDARD 11) # need at least c+11 standard
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
## Set warning flags
#set(WARN_FLAGS_TO_CHECK "") # checklist of warning flags
#set(WARN_FLAGS "") # actual warning flags to be added during compilation
## Add warning flags depending on options 
#if (YOSYS_ENABLE_NDEBUG)
#  set(WARN_FLAGS_TO_CHECK, ${WARN_FLAGS_TO_CHECK}, "-O3")
#endif()
#
#
##Collect the source files
#file(GLOB_RECURSE EXEC_YOSYS kernel/yosys.cc)
#file(GLOB_RECURSE LIB_SOURCES kernel/*.cc)
#file(GLOB_RECURSE LIB_HEADERS kernel/*.h)
#files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)
#
## Use c++ compiler for c source files
#set_source_files_properties(${LIB_SOURCES} PROPERTIES LANGUAGE CXX)
#set_source_files_properties(${EXEC_SOURCES} PROPERTIES LANGUAGE CXX)
#set_source_files_properties(${EXEC_SOURCES_SHELL} PROPERTIES LANGUAGE CXX)
#
##Build the library
#add_library(libyosys STATIC
#            ${LIB_HEADERS}
#            ${LIB_SOURCES})
#
## add header files to be included
#target_include_directories(libyosys PUBLIC ${LIB_INCLUDE_DIRS})
#set_target_properties(libyosys PROPERTIES PREFIX "") #Avoid extra 'lib' prefix#Create the executable
#
##Specify link-time dependancies
#target_link_libraries(libyosys
#                      readline)
#
## Build targets
## 1. yosys
#add_executable(yosys ${EXEC_SOURCES})
#target_link_libraries(vpr
#                      libyosys)
# 2. yosys-config

# run makefile provided, we pass-on the options to the local make file 
add_custom_target(
  yosys ALL 
  COMMAND 
    $(MAKE) 
    #CC=${CMAKE_C_COMPILER} 
    #CXX=${CMAKE_CXX_COMPILER} 
    #LD=${CMAKE_CXX_COMPILER} 
    #ENABLE_TCL=${YOSYS_ENABLE_TCL}
    #ENABLE_ABC=${YOSYS_ENABLE_ABC}
    #ENABLE_PLUGINS=${YOSYS_ENABLE_PLUGINS}
    #ENABLE_READLINE=${YOSYS_ENABLE_READLINE}
    #ENABLE_VERIFIC=${YOSYS_ENABLE_VERIFIC}
    #ENABLE_COVER=${YOSYS_ENABLE_COVER}
    #ENABLE_LIBYOSYS=${YOSYS_ENABLE_LIBYOSYS}
    #ENABLE_GPROF=${YOSYS_ENABLE_GPROF}
    #ENABLE_NDEBUG=${YOSYS_ENABLE_NDEBUG}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Compile Yosys with given Makefile"
)
