cmake_minimum_required(VERSION 3.9)

project("libodc")

# ODC, the constraints file of OpenFPGA (docs/spec_odc.md): the reader, the
# checks against a netlist and a device, and the writers of the files of the
# tools (SDC, the VPR constraints, the pin constraints, a Yosys fragment)
file(GLOB_RECURSE EXEC_SOURCES test/*.cpp)
file(GLOB_RECURSE LIB_SOURCES src/*.cpp)
file(GLOB_RECURSE LIB_HEADERS src/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCES})

add_library(libodc STATIC
            ${LIB_HEADERS}
            ${LIB_SOURCES})
target_include_directories(libodc PUBLIC ${LIB_INCLUDE_DIRS})
set_target_properties(libodc PROPERTIES PREFIX "") #Avoid extra 'lib' prefix

target_link_libraries(libodc
                      libvtrutil
                      libblifparse)

if (OPENFPGA_WITH_TEST)
  foreach(testsourcefile ${EXEC_SOURCES})
    get_filename_component(testname ${testsourcefile} NAME_WE)
    add_executable(${testname} ${testsourcefile})
    target_link_libraries(${testname} libodc)
    add_test(NAME ${testname} COMMAND ${testname})
  endforeach(testsourcefile ${EXEC_SOURCES})
endif()
