# Special for Yosys, which does not use CMake to install. Do manual install configuration here
include(GNUInstallDirs)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# Install dep libs
#install(FILES ${OPENSSL_INCLUDE_DIRS}/openssl/ssl.h
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#    COMPONENT dep_package
#)
install(FILES ${OPENSSL_LIBRARIES}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT openfpga_package
)
