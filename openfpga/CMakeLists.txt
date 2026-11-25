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

  if (OPENFPGA_SWIG_PYTHON)
    # Python module: import openfpga (built as _openfpga.so + openfpga.py)
    # The target cannot be called 'openfpga', which is the executable
    SwigLib(NAME        openfpga_python
            MODULE_NAME openfpga
            LANGUAGE    python
            I_FILE      src/openfpga_python.i
            # The Python code of the module is in python/, and is included by the .i file
            SWIG_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src
                          ${CMAKE_CURRENT_SOURCE_DIR}/python
            SWIG_DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/python/openfpga_python_diagnostics.py
                          ${CMAKE_CURRENT_SOURCE_DIR}/python/openfpga_python_session.py
                          ${CMAKE_CURRENT_SOURCE_DIR}/python/openfpga_python_tcl.py
                          ${CMAKE_CURRENT_SOURCE_DIR}/python/openfpga_python_worker.py)
    target_include_directories(openfpga_python PUBLIC ${LIB_INCLUDE_DIRS})
    target_link_libraries(openfpga_python
                          PUBLIC libopenfpga)
    # Built by pip (scikit-build-core): the module goes to the root of the
    # wheel, which is where the interpreter looks for 'import openfpga'
    if (SKBUILD)
      install(TARGETS openfpga_python
              LIBRARY DESTINATION .
              COMPONENT python)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/openfpga.py
              DESTINATION .
              COMPONENT python)
    endif()
  endif()
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
                        ${CMAKE_DL_LIBS}
                        )
endif()

# Plugins of the shell (docs/plan_shell_plugins.md). A plugin is a folder
# <name>/ with a CMakeLists.txt that calls openfpga_add_plugin(), found in
# openfpga/plugins or in a folder of OPENFPGA_PLUGIN_DIRS. Its library and
# libopenfpga need each other (it uses the shell, and the shell calls its
# register function), which CMake allows for static libraries
function(openfpga_add_plugin)
  cmake_parse_arguments(PLUGIN "" "NAME;VERSION" "SOURCES" ${ARGN})
  if (NOT PLUGIN_NAME OR NOT PLUGIN_VERSION OR NOT PLUGIN_SOURCES)
    message(FATAL_ERROR "openfpga_add_plugin() needs NAME, VERSION and SOURCES")
  endif()
  add_library(openfpga_plugin_${PLUGIN_NAME} STATIC ${PLUGIN_SOURCES})
  set_target_properties(openfpga_plugin_${PLUGIN_NAME} PROPERTIES PREFIX "")
  target_link_libraries(openfpga_plugin_${PLUGIN_NAME} PUBLIC libopenfpga)
  if (OPENFPGA_ENABLE_STRICT_COMPILE)
    target_compile_options(openfpga_plugin_${PLUGIN_NAME} PRIVATE -Werror)
  endif()
  set_property(GLOBAL PROPERTY OPENFPGA_PLUGIN_VERSION_${PLUGIN_NAME} "${PLUGIN_VERSION}")
endfunction()

set(OPENFPGA_PLUGIN_DECLARATIONS "")
set(OPENFPGA_PLUGIN_ENTRIES "")
set(OPENFPGA_PLUGINS_SEEN "")
foreach(plugin IN LISTS OPENFPGA_PLUGINS)
  if (NOT plugin MATCHES "^[a-z][a-z0-9_]*$")
    message(FATAL_ERROR "The name of the plugin '${plugin}' is not [a-z][a-z0-9_]*")
  endif()
  if (plugin IN_LIST OPENFPGA_PLUGINS_SEEN)
    message(FATAL_ERROR "The plugin '${plugin}' is given twice in OPENFPGA_PLUGINS")
  endif()
  list(APPEND OPENFPGA_PLUGINS_SEEN ${plugin})
  set(plugin_dir "")
  foreach(folder IN ITEMS ${CMAKE_CURRENT_SOURCE_DIR}/plugins ${OPENFPGA_PLUGIN_DIRS})
    if (NOT plugin_dir AND EXISTS ${folder}/${plugin}/CMakeLists.txt)
      set(plugin_dir ${folder}/${plugin})
    endif()
  endforeach()
  if (NOT plugin_dir)
    message(FATAL_ERROR "The plugin '${plugin}' is not in openfpga/plugins nor in OPENFPGA_PLUGIN_DIRS")
  endif()
  add_subdirectory(${plugin_dir} ${CMAKE_CURRENT_BINARY_DIR}/plugins/${plugin})
  if (NOT TARGET openfpga_plugin_${plugin})
    message(FATAL_ERROR "The plugin '${plugin}' (${plugin_dir}) does not call openfpga_add_plugin(NAME ${plugin} ...)")
  endif()
  get_property(plugin_version GLOBAL PROPERTY OPENFPGA_PLUGIN_VERSION_${plugin})
  target_link_libraries(libopenfpga openfpga_plugin_${plugin})
  string(APPEND OPENFPGA_PLUGIN_DECLARATIONS "void register_plugin_${plugin}(PluginApi& api);\n")
  string(APPEND OPENFPGA_PLUGIN_ENTRIES "    {{\"${plugin}\", \"${plugin_version}\"}, &register_plugin_${plugin}},\n")
  message(STATUS "OpenFPGA: plugin ${plugin} ${plugin_version} (${plugin_dir})")
endforeach()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/plugins/openfpga_plugin_list.cpp.in
               ${CMAKE_CURRENT_BINARY_DIR}/openfpga_plugin_list.cpp @ONLY)
target_sources(libopenfpga PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/openfpga_plugin_list.cpp)

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

# Unit tests, run by ctest
if (OPENFPGA_WITH_TEST)
  file(GLOB TEST_SOURCES test/*.cpp)
  foreach(testsourcefile ${TEST_SOURCES})
    get_filename_component(testname ${testsourcefile} NAME_WE)
    add_executable(${testname} ${testsourcefile})
    target_link_libraries(${testname} libopenfpga)
    # The tests read files of the repository, e.g., the sources of VPR
    target_compile_definitions(${testname} PRIVATE OPENFPGA_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
    add_test(NAME ${testname} COMMAND ${testname})
  endforeach()

  # The table of the options of VPR is generated from its sources: it must be
  # up to date
  find_package(Python3 QUIET COMPONENTS Interpreter)
  if (Python3_Interpreter_FOUND)
    add_test(NAME vpr_option_table_up_to_date
             COMMAND ${Python3_EXECUTABLE}
                     ${CMAKE_SOURCE_DIR}/openfpga_flow/scripts/generate_vpr_option_mapping.py --check)
  endif()
endif()
