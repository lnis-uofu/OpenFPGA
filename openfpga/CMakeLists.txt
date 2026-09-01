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
                      libmif
                      libpcf
                      libvtrutil
                      libbusgroup
                      libnamemanager
                      libtileconfig
                      libpugixml
                      libvpr
                      libopenfpgacapnproto
                      )
if (OPENFPGA_WITH_YOSYS STREQUAL "ON")
  # For yosys header files
  target_include_directories(libopenfpga PRIVATE 
      ${YOSYS_CMAKE_SOURCE_DIR}
      ${YOSYS_CMAKE_BINARY_DIR}
  )
  # 2. Bind the macros directly to that exact target name:
  target_compile_definitions(libopenfpga PRIVATE 
      _YOSYS_
      OPENFPGA_INCLUDE_YOSYS_COMMAND
  )
  add_dependencies(libopenfpga yosys)
  target_link_libraries(libopenfpga
                        libyosys
                        )
  # FIX: Handle MSVC runtime library mismatch
  if (MSVC)
    # Suppress the LIBCMT conflict warning and prefer MSVCRT
    target_link_options(libopenfpga PRIVATE /NODEFAULTLIB:LIBCMT)
  endif()
endif()

#Create the test executable
add_executable(openfpga ${EXEC_SOURCE})
target_link_libraries(openfpga libopenfpga)
if (OPENFPGA_WITH_YOSYS STREQUAL "ON")
  add_dependencies(openfpga yosys)
  target_include_directories(openfpga PRIVATE 
      ${YOSYS_CMAKE_SOURCE_DIR}
      ${YOSYS_CMAKE_BINARY_DIR}
  )
  # Define required directory and executable for yosys
  set(YOSYS_BUILD_SHARE_DIR    "${YOSYS_CMAKE_BINARY_DIR}/share")
  set(OPENFPGA_BUILD_SHARE_DIR "${CMAKE_CURRENT_BINARY_DIR}/share")
  # Path to the yosys-abc source and output locations
  set(YOSYS_SRC_ABC_EXE        "${YOSYS_CMAKE_BINARY_DIR}/yosys-abc")
  set(OPENFPGA_BUILD_ABC_EXE   "${CMAKE_CURRENT_BINARY_DIR}/yosys-abc")
  # Must copy/link after openfpga executable is built
  add_custom_command(
      TARGET openfpga
      POST_BUILD
      # Symlink Rule 1: Share folder directory
      COMMAND ${CMAKE_COMMAND} -E create_symlink "${YOSYS_BUILD_SHARE_DIR}" "${OPENFPGA_BUILD_SHARE_DIR}"
      # Symlink Rule 2: Yosys-ABC executable engine
      COMMAND ${CMAKE_COMMAND} -E create_symlink "${YOSYS_SRC_ABC_EXE}" "${OPENFPGA_BUILD_ABC_EXE}"
      COMMENT "Post-build: Creating necessary symlinks for Yosys and ABC environment"
      VERBATIM
  )
  # Special to compress yosys warnings
  message(STATUS "OpenFPGA: suppress warnings from Yosys codes")
  set(MY_TARGETS libopenfpga openfpga)
  foreach(target_name IN LISTS MY_TARGETS)
      # Get the current flags for the target
      get_target_property(CURRENT_FLAGS ${target_name} COMPILE_OPTIONS)
      
      # Check if the target actually has flags to avoid CMake errors
      if(CURRENT_FLAGS)
          # Remove the exact flag (change "-Wshadow" to your flag)
          list(REMOVE_ITEM CURRENT_FLAGS "-Wshadow" "-Wextra-semi" "-Wswitch-default")
          # Save the cleaned flags back to the target
          set_target_properties(${target_name} PROPERTIES COMPILE_OPTIONS "${CURRENT_FLAGS}")
      endif()
  endforeach()
endif()

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

deploy_runtime_dependencies(
    TARGETS libopenfpga openfpga
    DESTINATION bin
    COMPONENT openfpga_package
    RUNTIME_SET_NAME openfpga_shared_deps
)
