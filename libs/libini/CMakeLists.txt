cmake_minimum_required(VERSION 3.9)

project(libini)

file(GLOB_RECURSE LIB_HEADERS src/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

#Create the library
add_library(libini STATIC
            ${LIB_HEADERS})
target_link_libraries(libini)

target_include_directories(libini PUBLIC ${LIB_INCLUDE_DIRS})
set_target_properties(libini PROPERTIES PREFIX "" LINKER_LANGUAGE CXX)

install(TARGETS libini
        DESTINATION bin
        COMPONENT openfpga_package
)
