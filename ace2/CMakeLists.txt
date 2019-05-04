cmake_minimum_required(VERSION 2.8.12)

if (${CMAKE_VERSION} VERSION_GREATER "3.8")
    #For cmake >= 3.9 INTERPROCEDURAL_OPTIMIZATION behaviour we need to explicitly
    #set the cmake policy version number
    cmake_policy(VERSION 3.9) 

    # If we are using verison < 3.9 then setting INTERPROCEDURAL_OPTIMIZATION
    # has no effect unless an Intel compiler is used
endif()

project("ace2")

add_definitions(-DLIN)

#Collect the source files
file(GLOB_RECURSE EXEC_SOURCES SRC/ace.c)
file(GLOB_RECURSE LIB_SOURCES SRC/*.c)
file(GLOB_RECURSE LIB_HEADERS SRC/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

# Use c++ compiler for c source files
set_source_files_properties(${LIB_SOURCES} PROPERTIES LANGUAGE CXX)
set_source_files_properties(${EXEC_SOURCES} PROPERTIES LANGUAGE CXX)

# Remove test executable from library
list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCES}) 

#Create the library
add_library(libace STATIC
            ${LIB_HEADERS}
            ${LIB_SOURCES})
# add header files to be included
target_include_directories(libace PUBLIC ${LIB_INCLUDE_DIRS})
set_target_properties(libace PROPERTIES PREFIX "") #Avoid extra 'lib' prefix#Create the executable

# Specify dependency 
target_link_libraries(libace
                      libabc
                      ${CMAKE_DL_LIBS})

add_executable(ace ${EXEC_SOURCES})
target_link_libraries(ace libace)

#ACE uses some ABC functions (st__insert) which cause warnings - we suppress them here
set(ACE_SUPPRESS_FLAGS
        -Wno-int-to-pointer-cast
        )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON} -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS")

include(CheckCXXCompilerFlag)
foreach(flag ${ACE_SUPPRESS_FLAGS})
    CHECK_CXX_COMPILER_FLAG(${flag} CXX_COMPILER_SUPPORTS_${flag})
    if(CXX_COMPILER_SUPPORTS_${flag})
        target_compile_options(ace PRIVATE ${flag})
    endif()
endforeach()

install(TARGETS ace DESTINATION bin)
