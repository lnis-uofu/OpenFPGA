# Special for Yosys, which does not use CMake to install. Do manual install configuration here
include(GNUInstallDirs)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# Install dep libs
#install(FILES ${OPENSSL_INCLUDE_DIRS}/openssl/ssl.h
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#    COMPONENT dep_package
#)
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)
message("Found .so file at ${OPENSSL_LIBRARIES}")
foreach (src ${OPENSSL_LIBRARIES})
    if(IS_SYMLINK ${src})
        file(REAL_PATH ${src} TARGET_PATH)
        message("Follow the real path to .so file: ${src} ->  ${TARGET_PATH}")
        install(FILES ${TARGET_PATH}}
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT dep_package
        )
    endif()
    install(FILES ${OPENSSL_LIBRARIES}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT dep_package
    )
endforeach()
