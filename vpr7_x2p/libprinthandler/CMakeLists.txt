cmake_minimum_required(VERSION 2.8.12)

if (${CMAKE_VERSION} VERSION_GREATER "3.8")
    #For cmake >= 3.9 INTERPROCEDURAL_OPTIMIZATION behaviour we need to explicitly
    #set the cmake policy version number
    cmake_policy(VERSION 3.9) 

    # If we are using verison < 3.9 then setting INTERPROCEDURAL_OPTIMIZATION
    # has no effect unless an Intel compiler is used
endif()

# Set the default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build: None, Debug, Release, RelWithDebInfo, MinSizeRel"
        FORCE)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

#Create the project 
project("libprinthandler")

# set C compiler flag 
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -Wall -Wpointer-arith -Wcast-qual -ansi -pedantic -Wshadow -Wcast-allign -Wno-write-strings)

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
      "-Wall"
      "-Wpointer-arith"
      "-Wcast-qual"
      "-ansi"
      "-pedantic"
      "-Wshadow"
      "-Wcast-allign"
      "-Wno-write-strings"
      )
endif()

# compiler flag configuration checks
include(CheckCXXCompilerFlag)

# check and see if the compiler supports the various warning flags
# and add valid flags
foreach (flag ${WARN_FLAGS_TO_CHECK})
  CHECK_CXX_COMPILER_FLAG(${flag} CXX_COMPILER_SUPPORTS_${flag})
  if (C_COMPILER_SUPPORTS_${flag})
    # flag supported, so enable it
    set (WARN_FLAGS "${WARN_FLAGS} ${flag}")
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


#Collect the source files
file(GLOB_RECURSE EXEC_SOURCES SRC/main.cxx)
file(GLOB_RECURSE LIB_SOURCES SRC/*/*.cxx)
file(GLOB_RECURSE LIB_HEADERS SRC/*/*.h)
include_directories(SRC/TC_COMMON/ SRC/TIO_InputOutputHandlers/ ../pcre/SRC/)

#Create the library
add_library(libprinthandler STATIC
            ${LIB_HEADERS}
            ${LIB_SOURCES})

target_link_libraries(libprinthandler
                      libpcre)

target_include_directories(libprinthandler PUBLIC ${LIB_HEADERS})

#Create the executable
add_executable(printhandlerdemo ${EXEC_SOURCES})
target_link_libraries(printhandlerdemo 
                      libprinthandler)

