# Special for Yosys, which does not use CMake to install. Do manual install configuration here
include(GNUInstallDirs)
include(AutoPackageLib)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# Install dep libs
# SSL
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)
auto_package_lib(${OPENSSL_LIBRARIES})

# BOOST
find_package(Boost 1.87.0 CONFIG QUIET)
find_package(Boost REQUIRED COMPONENTS date_time unit_test_framework system filesystem)
auto_package_lib(${Boost_LIBRARIES})

# ZLIB
find_package(ZLIB REQUIRED)
auto_package_lib(${ZLIB_LIBRARIES})

find_package(FLEX REQUIRED)
auto_package_lib(${FLEX_LIBRARIES})

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(LIBFFI REQUIRED libffi)
#auto_package_lib(${LIBFFI_LIBRARIES})
