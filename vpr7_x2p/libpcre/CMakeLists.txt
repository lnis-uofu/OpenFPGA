cmake_minimum_required(VERSION 2.8.12)

if (${CMAKE_VERSION} VERSION_GREATER "3.8")
    #For cmake >= 3.9 INTERPROCEDURAL_OPTIMIZATION behaviour we need to explicitly
    #set the cmake policy version number
    cmake_policy(VERSION 3.9) 

    # If we are using verison < 3.9 then setting INTERPROCEDURAL_OPTIMIZATION
    # has no effect unless an Intel compiler is used
endif()

set(CMAKE_BUILD_TYPE=Release)

# set C compiler flag 
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -Wall -Wpointer-arith -Wcast-qual -ansi -pedantic -Wshadow -Wcast-allign -Wno-write-strings)

project("libpcre")

#Collect the source files
file(GLOB_RECURSE EXEC_SOURCES SRC/main.c)
file(GLOB_RECURSE LIB_SOURCES SRC/*.c)
file(GLOB_RECURSE LIB_HEADERS SRC/*.h)

#Create the library
add_library(libpcre STATIC
            ${LIB_HEADERS}
            ${LIB_SOURCES})

#Create the executable
add_executable(pcredemo ${EXEC_SOURCES})
target_link_libraries(pcredemo libpcre)

