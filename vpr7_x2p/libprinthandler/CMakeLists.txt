cmake_minimum_required(VERSION 2.8.12)

if (${CMAKE_VERSION} VERSION_GREATER "3.8")
    #For cmake >= 3.9 INTERPROCEDURAL_OPTIMIZATION behaviour we need to explicitly
    #set the cmake policy version number
    cmake_policy(VERSION 3.9) 

    # If we are using verison < 3.9 then setting INTERPROCEDURAL_OPTIMIZATION
    # has no effect unless an Intel compiler is used
endif()

#Create the project 
project("libprinthandler")

#Collect the source files
file(GLOB_RECURSE EXEC_SOURCES SRC/main.cxx)
file(GLOB_RECURSE LIB_SOURCES SRC/*/*.cxx)
file(GLOB_RECURSE LIB_HEADERS SRC/*/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

# Use c++ compiler for c source files
set_source_files_properties(${LIB_SOURCES} PROPERTIES LANGUAGE CXX)
set_source_files_properties(${EXEC_SOURCES} PROPERTIES LANGUAGE CXX)
set_source_files_properties(${EXEC_SOURCES_SHELL} PROPERTIES LANGUAGE CXX)

# Remove test executable from library
list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCES}) 

# Set final flags
# Local Flags
set (LOCAL_CXX_FLAGS "-DLINUX24_64 -DLINUX_X86_64")
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LOCAL_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")


#Create the library
add_library(libprinthandler STATIC
            ${LIB_HEADERS}
            ${LIB_SOURCES})

# add header files to be included
target_include_directories(libprinthandler PUBLIC ${LIB_INCLUDE_DIRS})
set_target_properties(libprinthandler PROPERTIES PREFIX "") #Avoid extra 'lib' prefix#Create the executable

target_link_libraries(libprinthandler
                      libpcre)

#Create the executable
add_executable(printhandlerdemo ${EXEC_SOURCES})
target_link_libraries(printhandlerdemo 
                      libprinthandler)

