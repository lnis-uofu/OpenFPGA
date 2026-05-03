cmake_minimum_required(VERSION 3.9)

project(libini)

file(GLOB_RECURSE LIB_HEADERS src/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

#Create the library
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.19")
    add_library(libini INTERFACE
                ${LIB_HEADERS})
else()
    add_library(libini INTERFACE)
endif()

target_include_directories(libini INTERFACE ${LIB_INCLUDE_DIRS})
set_target_properties(libini PROPERTIES PREFIX "")

install(TARGETS libini
        DESTINATION bin
        COMPONENT openfpga_package
)
