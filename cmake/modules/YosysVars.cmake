execute_process(
  COMMAND ${YOSYS_INSTALL}/bin/yosys-config --cxx
  OUTPUT_VARIABLE ENV{YS_CXX}
)
message(STATUS "++++++ YS_CXX: $ENV{YS_CXX}")
message(STATUS "++++++ YOSYS_INSTALL: ${YOSYS_INSTALL}")

execute_process(
  COMMAND ${YOSYS_INSTALL}/bin/yosys-config --cxxflags
  OUTPUT_VARIABLE YS_CXXFLAGS
)
message(STATUS "++++++ YS_CXXFLAGS: ${YS_CXXFLAGS}")

execute_process(
  COMMAND ${YOSYS_INSTALL}/bin/yosys-config --ldflags
  OUTPUT_VARIABLE YS_LDFLAGS
)
message(STATUS "++++++ YS_LDFLAGS: ${YS_LDFLAGS}")

execute_process(
  COMMAND ${YOSYS_INSTALL}/bin/yosys-config --ldlibs
  OUTPUT_VARIABLE YS_LDLIBS
)
message(STATUS "++++++ YS_LDLIBS: ${YS_LDLIBS}")

execute_process(
  COMMAND ${YOSYS_INSTALL}/bin/yosys-config --datdir
  OUTPUT_VARIABLE YS_DATA_DIR
)
message(STATUS "++++++ YS_DATA_DIR: ${YS_DATA_DIR}")

