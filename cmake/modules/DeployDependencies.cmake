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
            "^api-ms-.*"
            "^ext-ms-.*"
            "^kernel32\\.dll"
            "user32\\.dll"
            "msvcrt\\.dll"
            "ucrtbase\\.dll"
            "ntdll\\.dll"
            ".*[Aa]zure[Aa]ttest.*\\.dll"
            ".*[Hh]vsi[Ff]ile.*\\.dll"
            ".*[Pp]dm[Uu]tilities.*\\.dll"
            ".*[Ww][Tt][Dd][Ss][Ee][Nn][Ss][Oo][Rr].*\\.dll"
            "^[Cc]:/[Ww][Ii][Nn][Dd][Oo][Ww][Ss]/.*" # Ignores all standard Windows system DLL
        )
        # Robust check for MSYS2 contexts (MinGW, UCRT64, Clang64)
        if(MINGW OR CYGWIN OR DEFINED ENV{MSYSTEM})
            list(APPEND SYSTEM_EXCLUDE_REGEXES
                "^/usr/lib"
                "^/lib"
                "msys-.*\\.dll"
            )
        endif()
    else()
        set(SYSTEM_EXCLUDE_REGEXES
            "^/usr/lib"
            "^/lib"
            "^/lib64"
        )
    endif()

    # 2. Iterate through each target to configure RPATH and target installations
    foreach(current_target ${DEPLOY_TARGETS})
        if(NOT TARGET ${current_target})
            message(FATAL_ERROR "deploy_runtime_dependencies: '${current_target}' is not a valid CMake target.")
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

    # 3. Process and install the single unified dependency pool
    install(RUNTIME_DEPENDENCY_SET ${DEPLOY_RUNTIME_SET_NAME}
        DESTINATION ${DEPLOY_DESTINATION}
        COMPONENT ${DEPLOY_COMPONENT}
        PRE_EXCLUDE_REGEXES "${SYSTEM_EXCLUDE_REGEXES}"
        POST_EXCLUDE_REGEXES "${SYSTEM_EXCLUDE_REGEXES}"
        DIRECTORIES ${DEPLOY_ADDITIONAL_DIRECTORIES}
    )
endfunction()

