cmake_minimum_required(VERSION 3.9)

project("libopenfpgatileablerrgraph")

#Compiler info
set(OPENFPGA_COMPILER_INFO "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} on ${CMAKE_SYSTEM} ${CMAKE_SYSTEM_PROCESSOR}")

set(OPENFPGA_BUILD_INFO "${CMAKE_BUILD_TYPE}")

#Set default version numbers in case not specified
if(NOT DEFINED OPENFPGA_VERSION_MAJOR)
    set(OPENFPGA_VERSION_MAJOR 0)
endif()

if(NOT DEFINED OPENFPGA_VERSION_MINOR)
    set(OPENFPGA_VERSION_MINOR 0)
endif()

if(NOT DEFINED OPENFPGA_VERSION_PATCH)
    set(OPENFPGA_VERSION_PATCH 0)
endif()

set(OPENFPGA_BUILD_INFO "${OPENFPGA_BUILD_INFO} ASSERT_LEVEL=${VTR_ASSERT_LEVEL}")

# We always update the openfpga_version.cpp file every time the project is built,
# to ensure the git revision and dirty status are up to date.
#
# We need to do this in two stages:
#
# 1) We build a custom target 'version' (which is always out of date) so it will always be run.
#    It touches the unprocessed version input file so it too will always be out of date.
#
# 2) The custom command depends on the touched version input file and generates the processed
#    version file, with updated values. The custom command uses the configure_version.cmake
#    script to generate the up-to-date openfpga_version.cpp

#file(GLOB_RECURSE EXEC_SOURCES test/main.cpp)
file(GLOB_RECURSE LIB_SOURCES src/*.cpp)
file(GLOB_RECURSE LIB_HEADERS src/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

#Add vpr include files dirs
file(GLOB_RECURSE VPR_LIB_HEADERS ../../vpr/src/*.h)
files_to_dirs(VPR_LIB_HEADERS VPR_LIB_INCLUDE_DIRS)

#Add libs/librrgraph
file(GLOB_RECURSE RR_GRAPH_LIB_HEADERS ../../libs/librrgraph/src/base/*.h)
files_to_dirs(RR_GRAPH_LIB_HEADERS RR_GRAPH_LIB_INCLUDE_DIRS)

#Add openfpga
file(GLOB_RECURSE OPENFPGA_LIB_HEADERS ../../openfpga/src/base/*.h)
files_to_dirs(OPENFPGA_LIB_HEADERS OPENFPGA_LIB_INCLUDE_DIRS)

#Add openfpgautil
file(GLOB_RECURSE OPENFPGA_UTIL_LIB_HEADERS ../libopenfpgautil/src/*.h)
files_to_dirs(OPENFPGA_UTIL_LIB_HEADERS OPENFPGA_UTIL_LIB_INCLUDE_DIRS)

#Add tatum
#file(GLOB_RECURSE TATUM_LIB_HEADERS ../../libs/EXTERNAL/libtatum/libtatum/tatum/*.hpp)
#files_to_dirs(TATUM_LIB_HEADERS TATUM_LIB_INCLUDE_DIRS)
set(TATUM_LIB_INCLUDE_DIRS ../../libs/EXTERNAL/libtatum/libtatum)

#Add the version file to the sources
list(APPEND LIB_SOURCES ${OPENFPGA_VERSION_FILE_OUT})

#Remove test executable from library
#list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCES})

#Create the library
add_library(libopenfpgatileablerrgraph STATIC
            ${LIB_HEADERS}
            ${LIB_SOURCES})
        target_include_directories(libopenfpgatileablerrgraph PUBLIC ${LIB_INCLUDE_DIRS} ${VPR_LIB_INCLUDE_DIRS} ${RR_GRAPH_LIB_INCLUDE_DIRS} ${OPENFPGA_LIB_INCLUDE_DIRS} ${OPENFPGA_UTIL_LIB_INCLUDE_DIRS} ${TATUM_LIB_INCLUDE_DIRS})
set_target_properties(libopenfpgatileablerrgraph PROPERTIES PREFIX "") #Avoid extra 'lib' prefix

#Ensure version is always up to date by requiring version to be run first
add_dependencies(libopenfpgatileablerrgraph openfpga_version)

#Specify link-time dependancies
target_link_libraries(libopenfpgatileablerrgraph
                      libarchfpga
                      libvtrutil)

#Create the test executable
#add_executable(read_arch_openfpga ${EXEC_SOURCES})
#target_link_libraries(read_arch_openfpga libarchopenfpga)

#Supress IPO link warnings if IPO is enabled
#get_target_property(READ_ARCH_USES_IPO read_arch_openfpga INTERPROCEDURAL_OPTIMIZATION)
#if (READ_ARCH_USES_IPO)
#    set_target_properties(read_arch_openfpga PROPERTIES LINK_FLAGS ${IPO_LINK_WARN_SUPRESS_FLAGS})
#endif()

#install(TARGETS libarchopenfpga read_arch_openfpga DESTINATION bin)
