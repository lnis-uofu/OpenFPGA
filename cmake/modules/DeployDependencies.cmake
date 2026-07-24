## This function is the create runtime dependency set for an executable
# Call the custom module function 
# Pass multiple targets cleanly to your updated module
#deploy_runtime_dependencies(
#    TARGETS app_one app_two
#    DESTINATION bin
#    COMPONENT Runtime
#    RUNTIME_SET_NAME project_shared_deps # Optional customization parameter
#)
cmake_minimum_required(VERSION 3.21)

function(deploy_runtime_dependencies)
    set(options)
    set(oneValueArgs COMPONENT DESTINATION RUNTIME_SET_NAME)
    set(multiValueArgs TARGETS ADDITIONAL_DIRECTORIES)
    cmake_parse_arguments(DEPLOY "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Validate mandatory inputs
    if(NOT DEPLOY_TARGETS)
        message(FATAL_ERROR "deploy_runtime_dependencies: TARGETS argument is required.")
    endif()
    if(NOT DEPLOY_COMPONENT)
        set(DEPLOY_COMPONENT "Runtime")
    endif()
    if(NOT DEPLOY_DESTINATION)
        set(DEPLOY_DESTINATION "bin")
    endif()
    # Define a default namespace for the shared library bucket if none provided
    if(NOT DEPLOY_RUNTIME_SET_NAME)
        set(DEPLOY_RUNTIME_SET_NAME "global_runtime_deps")
    endif()

    # 1. Platform-specific regexes to ignore core Operating System libraries
    if(WIN32)
        set(SYSTEM_EXCLUDE_REGEXES
            "^[Cc]:/[Ww][Ii][Nn][Dd][Oo][Ww][Ss]"
            "^[Aa][Pp][Pp][Dd][Aa][Tt][Aa]"
            "api-ms-win-.*"           # Exclude Windows API forwarder DLLs
            "ext-ms-win-.*"           # Exclude extended Windows API DLLs
            "/[Ww][Ii][Nn][Dd][Oo][Ww][Ss]/"              # Exclude Windows system directories
            "kernel32"
            "user32"
            "msvcrt"
            "ucrtbase"
            "ntdll"
            ".*[Aa]zure[Aa]ttest.*"
            ".*[Hh]vsi[Ff]ile.*"
            ".*[Pp]dm[Uu]tilities.*"
            ".*[Ww][Tt][Dd][Ss][Ee][Nn][Ss][Oo][Rr].*"
            ".*[Ss]ystem32.*"
            ".*[Ss]ys[Ww][Oo][Ww]64.*"
        )
        # Fetch paths from environment variables if set, otherwise fallback to defaults
        if(DEFINED ENV{TCL_ROOT})
            set(TCL_BIN_DIR "$ENV{TCL_ROOT}/bin")
        else()
            set(TCL_BIN_DIR "C:/Tcl/bin")
        endif()
        
        if(DEFINED ENV{MSYSTEM_PREFIX})
            set(MSYS_BIN_DIR "$ENV{MSYSTEM_PREFIX}/bin")
        else()
            set(MSYS_BIN_DIR "C:/msys64/mingw64/bin")
        endif()
        # Robust check for MSYS2 contexts (MinGW, UCRT64, Clang64)
        if(MINGW OR CYGWIN OR DEFINED ENV{MSYSTEM})
            get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
            list(APPEND SYSTEM_EXCLUDE_REGEXES
                "^/usr/lib"
                "^/lib"
                "msys-.*"
            )
            set(DEPLOY_ADDITIONAL_DIRECTORIES
                ${MINGW_BIN_DIR}
                ${MSYS_BIN_DIR}
                ${TCL_BIN_DIR}      # Adds ActiveTcl (C:/Tcl/bin)
                "/usr/bin"
            )
            message(STATUS "DeployDependencies: Added default Mingw64 search directories")
        endif()
        # In MSVC build, tcl is installed by ActiveTcl
        # Adds ActiveTcl (C:/Tcl/bin)
        if (MSVC)
            set(DEPLOY_ADDITIONAL_DIRECTORIES
                ${TCL_BIN_DIR}
                # Magicsplat Tcl/Tk default installation locations
                "C:/Program Files/Tcl/bin"
                "C:/Program Files (x86)/Tcl/bin"
            )
        endif()
    else()
        set(SYSTEM_EXCLUDE_REGEXES
            "^/usr/lib"
            "^/lib"
            "^/lib64"
            "libc\\.so.*"
            "libpthread\\.so.*"
            "libglib-.*"
            "libdl\\.so.*"
            "libm\\.so.*"
            "librt\\.so.*"
            "libstdc\\+\\+\\.so.*"
            "libgcc_s\\.so.*"
            "libz\\.so.*$"
        )
    endif()

    # 2. For MSVC builds, add default search directories if not explicitly provided
    if(MSVC AND NOT DEPLOY_ADDITIONAL_DIRECTORIES)
        set(DEPLOY_ADDITIONAL_DIRECTORIES
            ${CMAKE_BINARY_DIR}/libs/libopenfpgashell/Release
            ${CMAKE_BINARY_DIR}/libs/libpcf/Release
            ${CMAKE_BINARY_DIR}/vtr-verilog-to-routing/vpr/Release
            ${CMAKE_BINARY_DIR}/vtr-verilog-to-routing/libs/libvtr/Release
            ${CMAKE_PREFIX_PATH}/bin
        )
        message(STATUS "DeployDependencies: Added default MSVC search directories")
    endif()

    # 3. Iterate through each target to configure RPATH and target installations
    foreach(current_target ${DEPLOY_TARGETS})
        if(NOT TARGET ${current_target})
            message(FATAL_ERROR "deploy_runtime_dependencies: '${current_target}' is not a valid CMake target.")
        endif()

        # Skip installation for static libraries as they don't have runtime dependencies
        get_target_property(target_type ${current_target} TYPE)
        if(target_type STREQUAL "STATIC_LIBRARY")
            continue()
        endif()

        # Automatic RPATH handling for Unix-like platforms
        if(UNIX AND NOT APPLE)
            set_target_properties(${current_target} PROPERTIES 
                INSTALL_RPATH "$ORIGIN"
            )
        endif()

        # Install target binary and append dependencies to the shared pool
        install(TARGETS ${current_target}
            RUNTIME_DEPENDENCY_SET ${DEPLOY_RUNTIME_SET_NAME}
            RUNTIME 
                DESTINATION ${DEPLOY_DESTINATION}
                COMPONENT ${DEPLOY_COMPONENT}
            LIBRARY 
                DESTINATION ${DEPLOY_DESTINATION}
                COMPONENT ${DEPLOY_COMPONENT}
        )
    endforeach()

    # 4. Process and install the single unified dependency pool
    install(RUNTIME_DEPENDENCY_SET ${DEPLOY_RUNTIME_SET_NAME}
        DESTINATION ${DEPLOY_DESTINATION}
        COMPONENT ${DEPLOY_COMPONENT}
        PRE_EXCLUDE_REGEXES ${SYSTEM_EXCLUDE_REGEXES}
        POST_EXCLUDE_REGEXES ${SYSTEM_EXCLUDE_REGEXES}
        DIRECTORIES ${DEPLOY_ADDITIONAL_DIRECTORIES}
    )
endfunction()
