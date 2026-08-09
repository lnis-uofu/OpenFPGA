# Special for Yosys, which does not use CMake to install. Do manual install configuration here
include(GNUInstallDirs)

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
