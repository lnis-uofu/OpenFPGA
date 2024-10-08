include(GNUInstallDirs)

if(NOT MSCV)
    # These flags generate noisy but non-bug warnings when using lib kj,
    # supress them.
    set(WARN_FLAGS_TO_DISABLE
        -Wno-undef
        -Wno-non-virtual-dtor
        )
    foreach(flag ${WARN_FLAGS_TO_DISABLE})
        CHECK_CXX_COMPILER_FLAG(${flag} CXX_COMPILER_SUPPORTS_${flag})
        if(CXX_COMPILER_SUPPORTS_${flag})
            #Flag supported, so enable it
            add_compile_options(${flag})
        endif()
    endforeach()
endif()

# Create generated headers from capnp schema files
#
# Each schema used should appear here.
set(CAPNP_DEFS
    gen/unique_blocks_uxsdcxx.capnp
)

capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS
    ${CAPNP_DEFS}
)


set(IC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../vtr-verilog-to-routing/libs/EXTERNAL/libinterchange/interchange)
set(CAPNPC_SRC_PREFIX ${IC_DIR})

find_program(WGET wget REQUIRED)
find_package(ZLIB REQUIRED)

# Add Java schema
set(JAVA_SCHEMA ${CMAKE_CURRENT_BINARY_DIR}/schema/capnp/java.capnp)
add_custom_command(
    OUTPUT ${JAVA_SCHEMA}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/schema/capnp/
    COMMAND ${WGET}
        https://raw.githubusercontent.com/capnproto/capnproto-java/master/compiler/src/main/schema/capnp/java.capnp
        -O ${JAVA_SCHEMA}
)


set(CAPNPC_IMPORT_DIRS)
list(APPEND CAPNPC_IMPORT_DIRS ${CMAKE_CURRENT_BINARY_DIR}/schema)

set(IC_PROTOS)
set(IC_SRCS)
set(IC_HDRS)
foreach(PROTO ${IC_PROTOS})
    capnp_generate_cpp(
        IC_SRC
        IC_HDR
        ${IC_DIR}/${PROTO}
    )
    list(APPEND IC_SRCS ${IC_SRC})
    list(APPEND IC_HDRS ${IC_HDR})
    list(APPEND CAPNP_DEFS ${IC_DIR}/${PROTO})
endforeach()


install(FILES ${CAPNP_DEFS} DESTINATION ${CMAKE_INSTALL_DATADIR}/openfpga)

add_library(libopenfpgacapnproto STATIC
            ${CAPNP_SRCS}
            ${IC_SRCS}
            mmap_file.h
            mmap_file.cpp
            serdes_utils.h
            serdes_utils.cpp
            )


add_dependencies(libopenfpgacapnproto
    get_java_capnp_schema)


target_include_directories(libopenfpgacapnproto PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/gen
    )
target_link_libraries(libopenfpgacapnproto
    libopenfpgautil
    CapnProto::capnp
)


add_custom_target(
    generate_unique_block_capnp
    COMMAND ${CMAKE_COMMAND} -E remove_directory unique_blocks_capnproto_generate
    COMMAND ${CMAKE_COMMAND} -E make_directory unique_blocks_capnproto_generate
    COMMAND ${CMAKE_COMMAND} -E chdir unique_blocks_capnproto_generate git clone https://github.com/duck2/uxsdcxx
    COMMAND python3 -mpip install --user -r unique_blocks_capnproto_generate/uxsdcxx/requirements.txt
    COMMAND ${CMAKE_COMMAND} -E chdir unique_blocks_capnproto_generate python3 uxsdcxx/uxsdcxx.py ${CMAKE_CURRENT_SOURCE_DIR}/gen/unique_blocks.xsd
    COMMAND ${CMAKE_COMMAND} -E copy
        unique_blocks_capnproto_generate/unique_blocks_uxsdcxx.h
        unique_blocks_capnproto_generate/unique_blocks_uxsdcxx_capnp.h
        unique_blocks_capnproto_generate/unique_blocks_uxsdcxx_interface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gen
    COMMAND ${CMAKE_COMMAND} -E copy unique_blocks_capnproto_generate/unique_blocks_uxsdcxx.capnp ${CMAKE_CURRENT_SOURCE_DIR}/gen
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/gen/unique_blocks.xsd
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
