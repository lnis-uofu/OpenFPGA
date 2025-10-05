cmake_minimum_required(VERSION 3.16)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if (${CMAKE_VERSION} VERSION_GREATER "3.8")
    #For cmake >= 3.9 INTERPROCEDURAL_OPTIMIZATION behaviour we need to explicitly
    #set the cmake policy version number
    cmake_policy(VERSION 3.9) 

    # If we are using verison < 3.9 then setting INTERPROCEDURAL_OPTIMIZATION
    # has no effect unless an Intel compiler is used
endif()

## Set the default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build: None, Debug, Release, RelWithDebInfo, MinSizeRel"
        FORCE)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
    message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds not allowed. Use the Makefile wrapper (e.g. make), or create a new build directory and call cmake manually from there (e.g. mkdir -p build && cd build && cmake .. && make). You may need to 'rm -rf CMakeCache.txt CMakeFiles' first.")
endif()

#We install to the source directory by default
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "default install path" FORCE)
endif()

set(OPENFPGA_IPO_BUILD "auto" CACHE STRING "Should OpenFPGA be compiled with interprocedural compiler optimizations?")
set_property(CACHE OPENFPGA_IPO_BUILD PROPERTY STRINGS auto on off)

#Allow the user to configure how much assertion checking should occur
set(VTR_ASSERT_LEVEL "2" CACHE STRING "VTR assertion checking level. 0: no assertions, 1: fast assertions, 2: regular assertions, 3: additional assertions with noticable run-time overhead, 4: all assertions (including those with significant run-time cost)")
set_property(CACHE VTR_ASSERT_LEVEL PROPERTY STRINGS 0 1 2 3 4)

#Create the project 
project("OpenFPGA-tool-suites" C CXX)

# Version number
file (STRINGS "VERSION.md" VERSION_NUMBER)
string (REPLACE "." ";" VERSION_LIST ${VERSION_NUMBER})
list(GET VERSION_LIST 0 OPENFPGA_VERSION_MAJOR)
list(GET VERSION_LIST 1 OPENFPGA_VERSION_MINOR)
list(GET VERSION_LIST 2 OPENFPGA_VERSION_PATCH)
set(OPENFPGA_VERSION_PRERELEASE "dev")

# Include user-defined functions
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
include(FilesToDirs)
include(SwigLib)

# Set the assertion level
add_definitions("-DVTR_ASSERT_LEVEL=${VTR_ASSERT_LEVEL}")

# compiler flag configuration checks
include(CheckCXXCompilerFlag)

# Options
## General options
option(OPENFPGA_WITH_YOSYS "Enable building Yosys" ON)
option(OPENFPGA_WITH_YOSYS_PLUGIN "Enable building Yosys plugin" ON)
option(OPENFPGA_WITH_TEST "Enable testing build for codebase. Once enabled, make test can be run" ON)
option(OPENFPGA_WITH_VERSION "Enable version always-up-to-date when building codebase. Disable only when you do not care an accurate version number" ON)
option(OPENFPGA_WITH_SWIG "Enable SWIG interface when building codebase. Disable when you do not need high-level interfaces, such as Tcl/Python" ON)
option(OPENFPGA_ENABLE_STRICT_COMPILE "Specifies whether compiler warnings should be treated as errors (e.g. -Werror)" OFF)
option(OPENFPGA_UPDATE_UNIQUE_BLOCK_CAPN "Specifies whether to update capn'proto files required by unique block. Recommend only after any modification on  unique block parsers and data structures" OFF)
## Installer options
option(OPENFPGA_GUI_INSTALLER "Enable installer with a graphical user interface" ON)
option(OPENFPGA_WITH_INSTALLER "Enable installer to be built" ON)
option(OPENFPGA_INSTALL_DOC "Installer will include documentation" ON)

message(STATUS "CMAKE_OPENFPGA_WITH_INSTALLER: ${OPENFPGA_WITH_INSTALLER}")
message(STATUS "CMAKE_OPENFPGA_INSTALL_DOC: ${OPENFPGA_INSTALL_DOC}")

# Options pass on to VTR
set(WITH_ABC ON CACHE BOOL "Enable building ABC in Verilog-to-Routing")
set(WITH_ODIN OFF CACHE BOOL "Enable building Odin in Verilog-to-Routing")
set(ODIN_DEBUG OFF CACHE BOOL "Enable building odin with debug flags in Verilog-to-Routing")
set(ODIN_WARN OFF CACHE BOOL "Enable building odin with extra warning flags in Verilog-to-Routing")
set(ODIN_COVERAGE OFF CACHE BOOL "Enable building odin with coverage flags in Verilog-to-Routing")
set(ODIN_TIDY OFF CACHE BOOL "Enable building odin with clang tidy in Verilog-to-Routing")
set(ODIN_SANITIZE OFF CACHE BOOL "Enable building odin with sanitize flags in Verilog-to-Routing")
set(WITH_YOSYS OFF CACHE BOOL "Enable building Yosys in Verilog-to-Routing")
set(WITH_PARMYS OFF CACHE BOOL "Enable Yosys as elaborator and parmys-plugin as partial mapper")
set(ODIN_YOSYS OFF CACHE BOOL "Enable building odin with yosys in Verilog-to-Routing")
set(YOSYS_SV_UHDM_PLUGIN OFF CACHE BOOL "Enable building and installing Yosys SystemVerilog and UHDM plugins in Verilog-to-Routing")
set(YOSYS_F4PGA_PLUGINS OFF CACHE BOOL "Enable building and installing Yosys SystemVerilog and UHDM plugins")
set(VTR_ENABLE_VERSION ${OPENFPGA_WITH_VERSION} CACHE BOOL "Enable version always-up-to-date when building codebase. Disable only when you do not care an accurate version number")
set(WITH_PARMYS OFF CACHE BOOL "Enable Yosys as elaborator and parmys-plugin as partial mapper")
# TODO: OpenFPGA and VPR has different requirements on no-warning build, e.g., on OS and compiler versions
#set(VTR_ENABLE_STRICT_COMPILE ${OPENFPGA_ENABLE_STRICT_COMPILE} CACHE BOOL "Specifies whether compiler warnings should be treated as errors (e.g. -Werror)")

# TCL file/lib required to link with SWIG generated wrapper
if (OPENFPGA_WITH_SWIG)
#Find Tcl
  include(FindTCL)
  message(STATUS "tcl.h path is : ${TCL_INCLUDE_PATH}")
  message(STATUS "libtcl.so path is : ${TCL_LIBRARY}") 

#Find SWIG
  find_package(SWIG 3.0 REQUIRED)
  if (SWIG_VERSION VERSION_GREATER_EQUAL "4.1.0")
    message(WARNING "Using SWIG >= ${SWIG_VERSION} -flatstaticmethod flag for python")
  endif()
  include(UseSWIG)
endif()

#Compiler flag configuration checks
include(CheckCXXCompilerFlag)

#
# We require c++20 support
#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #No compiler specific extensions

#
# Interprocedural optimization
#
#   Note that we manually clear the INTERPROCEDURAL_OPTIMIZATION flag on ABC later
#   to avoid cmake warnings
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
if (OPENFPGA_IPO_BUILD STREQUAL "on")
    if (IPO_SUPPORTED)
        message(STATUS "Building with IPO: on")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    else()
        message(ERROR "Building with IPO unsupported with this compiler!")
    endif()
elseif(OPENFPGA_IPO_BUILD STREQUAL "auto")
    if (IPO_SUPPORTED AND NOT CMAKE_BUILD_TYPE STREQUAL "debug")
        message(STATUS "Building with IPO: on (auto)")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    else()
        message(STATUS "Building with IPO: off (auto)")
    endif()
else()
    message(STATUS "Building with IPO: off")
endif()

if(NOT MSVC)
    # for GCC and Clang
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")
endif()

# Set WARN FLAGS
set(WARN_FLAGS "")

if (MSVC)
  # Visual studio warnings 
  # Note that we do not use /Wall since it generates warnings about standard library headers
  set(WARN_FLAGS_TO_CHECK # the flags to check if the compiler supports
      "/W4" # Most warnings
     )
else ()
  set(WARN_FLAGS_TO_CHECK # the flags to check if the compiler supports
      #GCC-like
      "-Wall"                         #Most warnings, typically good
      "-Wextra"                       #Extra warning, usually good
      "-Wpedantic"                    #Ensure ISO compliance (i.e. no non-standard extensions)
      "-Wcast-qual"                   #Warn if cast removes qualifier (e.g. const char* -> char*)
      "-Wcast-align"                  #Warn if a cast causes memory alignment changes
      "-Wshadow"                      #Warn if local variable shadows another variable
      "-Wformat=2"                    #Sanity checks for printf-like formatting
      "-Wno-format-nonliteral"        # But don't worry about non-literal formtting (i.e. run-time printf format strings)
      "-Wlogical-op"                  #Checks for logical op when bit-wise expected
      "-Wmissing-declarations"        #Warn if a global function is defined with no declaration
      "-Wmissing-include-dirs"        #Warn if a user include directory is missing
      "-Wredundant-decls"             #Warn if there are overlapping declarations
      "-Wswitch-default"              #Warn if a switch has no default
      "-Wundef"                       #Warn if #if() preprocessor refers to an undefined directive
      "-Wunused"                      #Warn about unused variables/parameters
      "-Wunused-variable"             #Warn about variables that are not used
      "-Wunused-parameter"            #Warn about function parameters which are unused
      "-Wdisabled-optimization"       #Warn when optimizations are skipped (usually due to large/complex code)
      "-Wnoexcept"                    #Warn when functions should be noexcept (i.e. compiler know it doesn't throw)
      "-Woverloaded-virtual"          #Warn when a function declaration overrides a virtual method
      "-Wctor-dtor-privacy"           #Warn about inaccessible constructors/destructors
      "-Wnon-virtual-dtor"            #Warn about missing virtual destructors
      "-Wduplicated-cond"             #Warn about identical conditions in if-else chains
      "-Wduplicated-branches"         #Warn when different branches of an if-else chain are equivalent
      "-Wnull-dereference"            #Warn about null pointer dereference execution paths
      "-Wuninitialized"               #Warn about unitialized values
      "-Winit-self"                   #Warn about self-initialization
      "-Wcatch-value=3"               #Warn when catch statements don't catch by reference
      "-Wextra-semi"                  #Warn about redudnant semicolons
      "-Wimplicit-fallthrough=3"      #Warn about case fallthroughs, but allow 'fallthrough' comments to suppress warnings
      #GCC-like optional
      #"-Wsuggest-final-types"         #Suggest where 'final' would help if specified on a type methods
      #"-Wsuggest-final-methods"       #Suggest where 'final' would help if specified on methods
      #"-Wsuggest-override"            #Suggest where 'override' should be specified
      #"-Wold-style-cast"              #Warn about using c-style casts
      #"-Wconversion"                  #Warn when type conversions may change value
      #"-Wsign-conversion"             #Warn if a conversion may change the sign
      #"-Wpadded"                      #Will warn if additional padding is introduced to a struct/class. Turn on if optimizing class memory layouts
      #"-Wstrict-overflow=2"           #Warn if the compiler optimizes assuming signed overflow does not occur
      #"-Wfloat-equal"                 #Warn about using direct floating point equality
      #"-Wunsafe-loop-optimizations"   #Warn when loops can't be optimized
      #"-Wswitch-enum"                 #Warn about uncovered enumeration values in a switch (even if there is a default)
      #"-Wsign-promo"                  #Warn when overload resolution converts an unsigned type to signed when an unsigned overload exists
      #"-Wdouble-promotion"            #Warn when float is implicitly propted to double
      #"-Wuseless-cast"                #Warn about casts to the same type
      #"-Wzero-as-null-pointer-constant" #Warn about using '0' instead of nullptr
      )
endif()

# check and see if the compiler supports the various warning flags
# and add valid flags
foreach (flag ${WARN_FLAGS_TO_CHECK})
  CHECK_CXX_COMPILER_FLAG(${flag} CXX_COMPILER_SUPPORTS_${flag})
  if (CXX_COMPILER_SUPPORTS_${flag})
    # flag supported, so enable it
    set (WARN_FLAGS "${WARN_FLAGS} ${flag}")
  endif()
endforeach()

#Suppress IPO link warnings
set(IPO_LINK_WARN_SUPRESS_FLAGS " ")
set(IPO_LINK_WARN_SUPRESS_FLAGS_TO_CHECK
    "-Wno-null-dereference"
    )
foreach(flag ${IPO_LINK_WARN_SUPRESS_FLAGS_TO_CHECK})
    CHECK_CXX_COMPILER_FLAG(${flag} CXX_COMPILER_SUPPORTS_${flag})
    if(CXX_COMPILER_SUPPORTS_${flag})
        #Flag supported, so enable it
        set(IPO_LINK_WARN_SUPRESS_FLAGS "${IPO_LINK_WARN_SUPRESS_FLAGS} ${flag}")
    endif()
endforeach()

#
# Sanitizer flags
#
set(SANITIZE_FLAGS "")
if(OPENFPGA_ENABLE_SANITIZE)
    #Enable sanitizers
    # -fuse-ld=gold force the gold linker to be used (required for sanitizers, but not enabled by default on some systems)
    set(SANITIZE_FLAGS "-g -fsanitize=address -fsanitize=leak -fsanitize=undefined -fuse-ld=gold")
    message(STATUS "SANTIIZE_FLAGS: ${SANITIZE_FLAGS}")
    link_libraries("-static-libasan") #Fixes 'ASan runtime does not come first in initial library list'
endif()

# Extra flags
set(SWIG_SHARED_FLAGS "")
if (OPENFPGA_WITH_SWIG)
    set(SWIG_SHARED_FLAGS "-fpic")
endif()
add_compile_options(${SWIG_SHARED_FLAGS})

# Set final flags
#
separate_arguments(
    ADDITIONAL_FLAGS UNIX_COMMAND "${SANITIZE_FLAGS} ${PROFILING_FLAGS} ${COVERAGE_FLAGS} ${LOGGING_FLAGS} ${COLORED_COMPILE} ${EXTRA_FLAGS} ${SWIG_SHARED_FLAGS}"
    )
separate_arguments(
    WARN_FLAGS UNIX_COMMAND "${WARN_FLAGS}"
    )

# 
# Sub-projects with their own compiler settings
#
add_subdirectory(vtr-verilog-to-routing)

add_compile_options(${WARN_FLAGS}) #Add warn flags for VTR tools
add_compile_options(${ADDITIONAL_FLAGS})
link_libraries(${ADDITIONAL_FLAGS})

# Unit Testing
#
if (OPENFPGA_WITH_TEST)
    enable_testing()
endif()

# 
# Sub-projects to apply current complier settings
#
add_subdirectory(libs)
add_subdirectory(openfpga)

# yosys compilation starts

# Compilation options for yosys
include(CMakeParseArguments)

##project(yosys)

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

#PugiXml has some deliberate switch fallthrough cases (as indicated by comments), but they
#are tagged as warnings with g++-7 (the comments don't match g++-7's suppression regexes).
#Since we don't want to change PugiXml (it is developed externally), we relax the warning
#level so no fallthrough warnings are generated
CHECK_CXX_COMPILER_FLAG("-Wimplicit-fallthrough=0" CXX_COMPILER_SUPPORTS_-Wimplicit-fallthrough=0)
if(CXX_COMPILER_SUPPORTS_-Wimplicit-fallthrough=0)
    target_compile_options(libpugixml PRIVATE "-Wimplicit-fallthrough=0")
endif()

# we will check if yosys already exist. if not then build it
if (OPENFPGA_WITH_YOSYS)
    if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/yosys/bin/yosys)
        message(STATUS "Yosys pre-build exist so skipping it")
    else ()
        # run makefile provided, we pass-on the options to the local make file 
        add_custom_target(
            yosys ALL
            # add step to remove the 'built-in' quicklogic plugins code in yosys
            COMMAND rm -rf techlibs/quicklogic
            COMMAND $(MAKE) config-gcc
            COMMAND $(MAKE) install PREFIX=${CMAKE_CURRENT_BINARY_DIR}/yosys/
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/yosys
            COMMENT "Compile Yosys with given Makefile"
        )
        # yosys compilation ends
        # yosys-plugins compilation starts
        add_custom_target(
            yosys-slang ALL
            COMMAND $(MAKE) install YOSYS_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/yosys/bin/ CMAKE_FLAGS+="-DPASS_NAME=read_slang"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/yosys-slang
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/yosys/bin/yosys
            COMMENT "Compile Yosys-Slang with given Makefile"
        )
        add_dependencies(yosys-slang yosys)

        ## yosys-plugins compilation starts
        #if (OPENFPGA_WITH_YOSYS_PLUGIN)
        #    add_custom_target(
        #        yosys-plugins ALL
        #        COMMAND $(MAKE) install_ql-qlf YOSYS_PATH=${CMAKE_CURRENT_BINARY_DIR}/yosys EXTRA_FLAGS="-DPASS_NAME=synth_ql"
        #        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/yosys-plugins
        #        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/yosys/bin/yosys
        #        COMMENT "Compile Yosys-plugins with given Makefile"
        #    )
        #    add_dependencies(yosys-plugins yosys)
        #endif()
    endif()
endif()

# run make to extract compiler options, linker options and list of source files
#add_custom_target(
#  yosys
#  COMMAND make run
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/yosys
#)

# Add extra compilation flags to suppress warnings from some libraries/tools
# Note that target_compile_options() *appends* to the current compilation options of
# the specified target

# For installer
if (OPENFPGA_WITH_INSTALLER)
  include(OpenfpgaInstaller)
  include(OpenfpgaPackaging)
endif()
