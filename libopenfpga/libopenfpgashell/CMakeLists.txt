cmake_minimum_required(VERSION 3.9)

project("libopenfpgashell")

file(GLOB_RECURSE EXEC_SOURCES test/test_command_parser.cpp)
file(GLOB_RECURSE LIB_SOURCES src/*.cpp)
file(GLOB_RECURSE LIB_HEADERS src/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

#Remove test executable from library
list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCES})

#Create the library
add_library(libopenfpgashell STATIC
             ${LIB_HEADERS}
             ${LIB_SOURCES})
target_include_directories(libopenfpgashell PUBLIC ${LIB_INCLUDE_DIRS})
set_target_properties(libopenfpgashell PROPERTIES PREFIX "") #Avoid extra 'lib' prefix

#Specify link-time dependancies
target_link_libraries(libopenfpgashell
                      libopenfpgautil
                      libvtrutil)

#Create the test executable
add_executable(test_command_parser ${EXEC_SOURCES})
target_link_libraries(test_command_parser libopenfpgashell)

#Supress IPO link warnings if IPO is enabled
#get_target_property(READ_ARCH_USES_IPO read_arch_openfpga INTERPROCEDURAL_OPTIMIZATION)
#if (READ_ARCH_USES_IPO)
#    set_target_properties(read_arch_openfpga PROPERTIES LINK_FLAGS ${IPO_LINK_WARN_SUPRESS_FLAGS})
#endif()

#install(TARGETS libarchopenfpga read_arch_openfpga DESTINATION bin)
