cmake_minimum_required(VERSION 3.9)

project("libmif")

file(GLOB_RECURSE EXEC_SOURCES test/*.cpp)
file(GLOB_RECURSE LIB_SOURCES src/*/*.cpp)
file(GLOB_RECURSE LIB_HEADERS src/*/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

#Remove test executable from library
list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCES})

#Create the library
add_library(libmif STATIC
            ${LIB_HEADERS}
            ${LIB_SOURCES})
target_include_directories(libmif PUBLIC ${LIB_INCLUDE_DIRS})
set_target_properties(libmif PROPERTIES PREFIX "") #Avoid extra 'lib' prefix
target_link_libraries(libmif
                      libopenfpgashell
                      libvtrutil)

#Create the test executable
foreach(testsourcefile ${EXEC_SOURCES})
    get_filename_component(testname ${testsourcefile} NAME_WE)
    add_executable(${testname} ${testsourcefile})
    target_link_libraries(${testname} libmif libvtrutil)
    add_test(NAME ${testname}
             COMMAND ${testname}
                     ${CMAKE_CURRENT_SOURCE_DIR}/example/src/test.mif
                     ${CMAKE_CURRENT_BINARY_DIR}/test_mif_out.mif)
endforeach(testsourcefile ${EXEC_SOURCES})

install(TARGETS libmif
        DESTINATION bin
        COMPONENT openfpga_package
)
