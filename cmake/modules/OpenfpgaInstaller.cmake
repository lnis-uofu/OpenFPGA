# Special for Yosys, which does not use CMake to install. Do manual install configuration here
include(GNUInstallDirs)

if (OPENFPGA_WITH_YOSYS)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/yosys/bin/ 
          DESTINATION ${CMAKE_INSTALL_BINDIR}
          COMPONENT openfpga_package
          PATTERN "*"
          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
  )
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/yosys/share/ 
          DESTINATION ${CMAKE_INSTALL_DATADIR}
          COMPONENT openfpga_package
  )
endif()
  
# Include all the documentation
if (OPENFPGA_INSTALL_DOC STREQUAL "ON")
  message("Include documentation in installer")
  install(DIRECTORY docs/build
          DESTINATION ${CMAKE_INSTALL_DOCDIR}
          COMPONENT openfpga_doc_package
  )
endif()

# Include licenses
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
        DESTINATION "."
        COMPONENT openfpga_package
)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_BINDIR}")
## Use install(CODE) to run file(GET_RUNTIME_DEPENDENCIES) at install time
#install(CODE [[
#  file(GET_RUNTIME_DEPENDENCIES
#    EXECUTABLES $<TARGET_FILE:vpr>
#    RESOLVED_DEPENDENCIES_VAR resolved_deps
#    UNRESOLVED_DEPENDENCIES_VAR unresolved_deps
#  )
#
#  foreach(dep ${resolved_deps})
#    message(INFO "Resolved dependency: ${dep}")
#    file(INSTALL FILES "${dep}" DESTINATION "${INSTALL_BIN_DIR}")
#  endforeach()
#
#  if(unresolved_deps)
#    message(WARNING "Unresolved dependencies detected: ${unresolved_deps}")
#  endif()
#]])
