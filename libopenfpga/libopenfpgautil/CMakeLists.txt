cmake_minimum_required(VERSION 3.9)

project("libopenfpgautil")

#Version info
set(OPENFPGA_VERSION_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/src/openfpga_version.cpp.in)
set(OPENFPGA_VERSION_FILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/openfpga_version.cpp)

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
add_custom_target(openfpga_version ALL
    COMMAND ${CMAKE_COMMAND} -E touch ${OPENFPGA_VERSION_FILE_IN})

add_custom_command(OUTPUT ${OPENFPGA_VERSION_FILE_OUT}
    COMMAND ${CMAKE_COMMAND} 
                    -D IN_FILE=${OPENFPGA_VERSION_FILE_IN}
                    -D OUT_FILE=${OPENFPGA_VERSION_FILE_OUT}
                    -D OPENFPGA_VERSION_MAJOR=${OPENFPGA_VERSION_MAJOR}
                    -D OPENFPGA_VERSION_MINOR=${OPENFPGA_VERSION_MINOR}
                    -D OPENFPGA_VERSION_PATCH=${OPENFPGA_VERSION_PATCH}
                    -D OPENFPGA_VERSION_PRERELEASE=${OPENFPGA_VERSION_PRERELEASE}
                    -D OPENFPGA_COMPILER_INFO=${OPENFPGA_COMPILER_INFO}
                    -D OPENFPGA_BUILD_INFO=${OPENFPGA_BUILD_INFO}
                    -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/configure_version.cmake
    MAIN_DEPENDENCY ${OPENFPGA_VERSION_FILE_IN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	VERBATIM)

#file(GLOB_RECURSE EXEC_SOURCES test/main.cpp)
file(GLOB_RECURSE LIB_SOURCES src/*.cpp)
file(GLOB_RECURSE LIB_HEADERS src/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

#Add the version file to the sources
list(APPEND LIB_SOURCES ${OPENFPGA_VERSION_FILE_OUT})

#Remove test executable from library
#list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCES})

#Create the library
add_library(libopenfpgautil STATIC
            ${LIB_HEADERS}
            ${LIB_SOURCES})
target_include_directories(libopenfpgautil PUBLIC ${LIB_INCLUDE_DIRS})
set_target_properties(libopenfpgautil PROPERTIES PREFIX "") #Avoid extra 'lib' prefix

#Ensure version is always up to date by requiring version to be run first
add_dependencies(libopenfpgautil openfpga_version)

#Specify link-time dependancies
target_link_libraries(libopenfpgautil
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
