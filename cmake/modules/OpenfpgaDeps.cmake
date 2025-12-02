# Special for Yosys, which does not use CMake to install. Do manual install configuration here
include(GNUInstallDirs)
include(AutoPackageLib)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# Install dep libs
# SSL
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)
message("Found .so file at ${OPENSSL_LIBRARIES}")
auto_package_lib(${OPENSSL_LIBRARIES})
