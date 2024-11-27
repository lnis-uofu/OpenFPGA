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
set(CAPNP_DEFS
    gen/unique_blocks_uxsdcxx.capnp
)

capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS
    ${CAPNP_DEFS}
)



add_library(libopenfpgacapnproto STATIC
            ${CAPNP_SRCS}
            ${IC_SRCS}
            )

if (OPENFPGA_UPDATE_UNIQUE_BLOCK_CAPN)
  add_dependencies(libopenfpgacapnproto
      generate_unique_block_capnp
      )
endif()

target_include_directories(libopenfpgacapnproto PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/gen
    )
target_link_libraries(libopenfpgacapnproto
    libopenfpgautil
    libvtrcapnproto
)


add_custom_target(
    generate_unique_block_capnp
    COMMAND ${CMAKE_COMMAND} -E remove_directory unique_blocks_capnproto_generate
    COMMAND ${CMAKE_COMMAND} -E make_directory unique_blocks_capnproto_generate
    COMMAND ${CMAKE_COMMAND} -E chdir unique_blocks_capnproto_generate git clone https://github.com/duck2/uxsdcxx
    COMMAND python3 -mpip install --user -r unique_blocks_capnproto_generate/uxsdcxx/requirements.txt
    COMMAND ${CMAKE_COMMAND} -E chdir unique_blocks_capnproto_generate python3 uxsdcxx/uxsdcxx.py ${CMAKE_CURRENT_SOURCE_DIR}/gen/unique_blocks.xsd
    COMMAND ${CMAKE_COMMAND} -E chdir unique_blocks_capnproto_generate python3 uxsdcxx/uxsdcap.py ${CMAKE_CURRENT_SOURCE_DIR}/gen/unique_blocks.xsd
        unique_blocks_capnproto_generate/unique_blocks_uxsdcxx.h
        unique_blocks_capnproto_generate/unique_blocks_uxsdcxx_capnp.h
        unique_blocks_capnproto_generate/unique_blocks_uxsdcxx_interface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gen
    COMMAND ${CMAKE_COMMAND} -E copy unique_blocks_capnproto_generate/unique_blocks_uxsdcxx.capnp ${CMAKE_CURRENT_SOURCE_DIR}/gen
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/gen/unique_blocks.xsd
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
