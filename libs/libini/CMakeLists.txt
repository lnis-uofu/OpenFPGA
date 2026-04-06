cmake_minimum_required(VERSION 3.9)

project(libini)

file(GLOB_RECURSE LIB_HEADERS src/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

# Create a header-only interface library
add_library(libini INTERFACE)

target_include_directories(libini INTERFACE ${LIB_INCLUDE_DIRS})

install(DIRECTORY src/
        DESTINATION bin
        COMPONENT openfpga_package
)