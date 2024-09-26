cmake_minimum_required(VERSION 3.9)

project("openfpga")

file(GLOB_RECURSE EXEC_SOURCE src/main.cpp)
file(GLOB_RECURSE LIB_SOURCES src/*/*.cpp)
file(GLOB_RECURSE LIB_HEADERS src/*/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

#Remove test executable from library
list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCE})

if (OPENFPGA_WITH_SWIG)
# SWIG library
  SwigLib(NAME      openfpga_shell
          NAMESPACE std
          LANGUAGE  tcl
          I_FILE    src/openfpga_shell.i)
  target_include_directories(openfpga_shell PUBLIC ${LIB_INCLUDE_DIRS})
  target_link_libraries(openfpga_shell
                        libopenfpga)
endif()

#Create the library
#Static linked library for other C++ libraries
add_library(libopenfpga STATIC
            ${LIB_HEADERS}
            ${LIB_SOURCES})
target_include_directories(libopenfpga PUBLIC ${LIB_INCLUDE_DIRS})
set_target_properties(libopenfpga PROPERTIES PREFIX "") #Avoid extra 'lib' prefix

#Specify link-time dependancies
target_link_libraries(libopenfpga
                      libclkarchopenfpga
                      libarchopenfpga
                      libopenfpgashell
                      libopenfpgautil
                      libfabrickey
                      libfpgabitstream
                      libini
                      libpcf
                      libvtrutil
                      libbusgroup
                      libnamemanager
                      libtileconfig
                      libpugixml
                      libvpr
                      libopenfpgacapnproto
                      )

#Create the test executable
add_executable(openfpga ${EXEC_SOURCE})
target_link_libraries(openfpga libopenfpga)

if (OPENFPGA_ENABLE_STRICT_COMPILE)
    message(STATUS "OpenFPGA: building with strict flags")

    set(OPENFPGA_STRICT_COMPILE_FLAGS_TO_CHECK
        #GCC-like
        "-Werror"
        # due to the pointer hackery in timing_driven_route_structs and BinaryHeap.heap_
        "-Wno-error=free-nonheap-object"
        )

    foreach(flag ${OPENFPGA_STRICT_COMPILE_FLAGS_TO_CHECK})
        message(STATUS "\tAdding CXX flag: ${flag}")
        target_compile_options(libopenfpga PRIVATE ${flag})
        target_compile_options(openfpga PRIVATE ${flag})
        target_link_libraries(openfpga ${flag})
    endforeach()
endif()

#Suppress IPO link warnings if IPO is enabled
get_target_property(OPENFPGA_USES_IPO openfpga INTERPROCEDURAL_OPTIMIZATION)
if (OPENFPGA_USES_IPO)
    set_property(TARGET openfpga APPEND PROPERTY LINK_FLAGS ${IPO_LINK_WARN_SUPRESS_FLAGS})
endif()

install(TARGETS libopenfpga openfpga DESTINATION bin)
