# An example of a plugin of the shell, built only when OPENFPGA_PLUGINS has
# 'example' (CI builds it to test the plugins). It adds a command, overrides
# one, removes one, and hooks some (see example_plugin.cpp)
openfpga_add_plugin(NAME example
                    VERSION 1.0
                    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/example_plugin.cpp)

if (OPENFPGA_WITH_TEST)
  add_executable(test_example_plugin ${CMAKE_CURRENT_SOURCE_DIR}/test_example_plugin.cpp)
  target_link_libraries(test_example_plugin libopenfpga)
  target_compile_definitions(test_example_plugin PRIVATE OPENFPGA_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
  add_test(NAME test_example_plugin COMMAND test_example_plugin)
endif()
