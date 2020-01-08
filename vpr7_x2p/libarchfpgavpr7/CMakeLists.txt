cmake_minimum_required(VERSION 2.8.12)

if (${CMAKE_VERSION} VERSION_GREATER "3.8")
    #For cmake >= 3.9 INTERPROCEDURAL_OPTIMIZATION behaviour we need to explicitly
    #set the cmake policy version number
    cmake_policy(VERSION 3.9) 

    # If we are using verison < 3.9 then setting INTERPROCEDURAL_OPTIMIZATION
    # has no effect unless an Intel compiler is used
endif()

project("libarchfpgavpr7")

#Collect the source files
file(GLOB_RECURSE EXEC_SOURCES SRC/main.c)
file(GLOB_RECURSE LIB_SOURCES SRC/*.c SRC/*/*.c SRC/*.cpp SRC/*/*.cpp)
file(GLOB_RECURSE LIB_HEADERS SRC/*.h SRC/*/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

# Use c++ compiler for c source files
set_source_files_properties(${LIB_SOURCES} PROPERTIES LANGUAGE CXX)
set_source_files_properties(${EXEC_SOURCES} PROPERTIES LANGUAGE CXX)
set_source_files_properties(${EXEC_SOURCES_SHELL} PROPERTIES LANGUAGE CXX)

# Remove test executable from library
list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCES}) 

#Create the library
add_library(libarchfpgavpr7 STATIC
            ${LIB_HEADERS}
            ${LIB_SOURCES})
# add header files to be included
target_include_directories(libarchfpgavpr7 PUBLIC ${LIB_INCLUDE_DIRS})
set_target_properties(libarchfpgavpr7 PROPERTIES PREFIX "") #Avoid extra 'lib' prefix#Create the executable

# Specify dependency 
target_link_libraries(libarchfpgavpr7
                      libvtrutil
                      libpcre
                      libprinthandler)

add_executable(read_arch_vpr7 ${EXEC_SOURCES})
target_link_libraries(read_arch_vpr7 libarchfpgavpr7 libvtrutil)


# install: TO BE TESTED
#install(TARGETS libarchfpga_vpr7 read_arch_vpr7 DESTINATION bin)
