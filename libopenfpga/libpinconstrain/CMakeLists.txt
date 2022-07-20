cmake_minimum_required(VERSION 3.9)

project("libpinconstrain")

file(GLOB_RECURSE EXEC_SOURCES test/*.cpp)
file(GLOB_RECURSE LIB_SOURCES src/*.cpp)
file(GLOB_RECURSE LIB_HEADERS src/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

file(GLOB_RECURSE BLIF_READ_HEADERS ../../vpr/src/base/*.h)
files_to_dirs(BLIF_READ_HEADERS BLIF_READ_INCLUDE_DIRS)

file(GLOB_RECURSE VTR_UTIL_HEADERS ../../libs/libvtrutil/src/*.h)
files_to_dirs(VTR_UTIL_HEADERS VTR_UTIL_INCLUDE_DIRS)

#Remove test executable from library
list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCES})

#Create the library
add_library(libpinconstrain STATIC
             ${LIB_HEADERS}
             ${LIB_SOURCES})
target_include_directories(libpinconstrain PUBLIC ${LIB_INCLUDE_DIRS} ${BLIF_READ_INCLUDE_DIRS} ${VTR_UTIL_INCLUDE_DIRS})
set_target_properties(libpinconstrain PROPERTIES PREFIX "") #Avoid extra 'lib' prefix

#Specify link-time dependancies
target_link_libraries(libpinconstrain
                      libvpr
                      libvtrutil
                      libpugixml)

#Create the test executable
foreach(testsourcefile ${EXEC_SOURCES})
    # Use a simple string replace, to cut off .cpp.
    get_filename_component(testname ${testsourcefile} NAME_WE)
    add_executable(${testname} ${testsourcefile})
    target_include_directories(${testname} PUBLIC ${LIB_INCLUDE_DIRS} ${BLIF_READ_INCLUDE_DIRS} ${VTR_UTIL_INCLUDE_DIRS})
    # Make sure the library is linked to each test executable
    target_link_libraries(${testname} libpinconstrain)
endforeach(testsourcefile ${EXEC_SOURCES})
