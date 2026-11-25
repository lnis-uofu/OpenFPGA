# The ui plugin: the commands that the Python module, the MCP server and the
# app use to read the flow and set its options (the queries and the
# application options). It is on by default (OPENFPGA_PLUGINS=ui), and the
# Python module needs it
file(GLOB UI_PLUGIN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
openfpga_add_plugin(NAME ui
                    VERSION 1.0
                    SOURCES ${UI_PLUGIN_SOURCES})
target_include_directories(openfpga_plugin_ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# The constraints of ODC (docs/spec_odc.md)
target_link_libraries(openfpga_plugin_ui PUBLIC libodc)

if (OPENFPGA_WITH_TEST)
  file(GLOB UI_PLUGIN_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)
  foreach(testsourcefile ${UI_PLUGIN_TESTS})
    get_filename_component(testname ${testsourcefile} NAME_WE)
    add_executable(${testname} ${testsourcefile})
    target_link_libraries(${testname} libopenfpga)
    add_test(NAME ${testname} COMMAND ${testname})
  endforeach()
endif()
