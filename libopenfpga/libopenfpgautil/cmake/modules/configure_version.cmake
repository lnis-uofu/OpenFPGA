#
# Versioning information
#
#Figure out the git revision
find_package(Git QUIET)
if(GIT_FOUND)
    exec_program(${GIT_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}
                 ARGS describe --always --long --dirty
                 OUTPUT_VARIABLE OPENFPGA_VCS_REVISION
                 RETURN_VALUE GIT_DESCRIBE_RETURN_VALUE)

    if(NOT GIT_DESCRIBE_RETURN_VALUE EQUAL 0)
        #Git describe failed, usually this means we
        #aren't in a git repo -- so don't set a VCS 
        #revision
        set(OPENFPGA_VCS_REVISION "unkown")
    endif()

    #Call again with exclude to get the revision excluding any tags
    #(i.e. just the commit ID and dirty flag)
    exec_program(${GIT_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}
                 ARGS describe --always --long --dirty --exclude '*'
                 OUTPUT_VARIABLE OPENFPGA_VCS_REVISION_SHORT
                 RETURN_VALUE GIT_DESCRIBE_RETURN_VALUE)
    if(NOT GIT_DESCRIBE_RETURN_VALUE EQUAL 0)
        #Git describe failed, usually this means we
        #aren't in a git repo -- so don't set a VCS 
        #revision
        set(OPENFPGA_VCS_REVISION_SHORT "unkown")
    endif()
else()
    #Couldn't find git, so can't look-up VCS revision
    set(OPENFPGA_VCS_REVISION "unkown")
    set(OPENFPGA_VCS_REVISION_SHORT "unkown")
endif()


#Set the version according to semver.org
set(OPENFPGA_VERSION "${OPENFPGA_VERSION_MAJOR}.${OPENFPGA_VERSION_MINOR}.${OPENFPGA_VERSION_PATCH}")
if(OPENFPGA_VERSION_PRERELEASE)
    set(OPENFPGA_VERSION "${OPENFPGA_VERSION}-${OPENFPGA_VERSION_PRERELEASE}")
endif()
set(OPENFPGA_VERSION_SHORT ${OPENFPGA_VERSION})
if(OPENFPGA_VCS_REVISION)
    set(OPENFPGA_VERSION "${OPENFPGA_VERSION}+${OPENFPGA_VCS_REVISION_SHORT}")
endif()

#Other build meta-data
string(TIMESTAMP OPENFPGA_BUILD_TIMESTAMP)
set(OPENFPGA_BUILD_TIMESTAMP "${OPENFPGA_BUILD_TIMESTAMP}")
set(OPENFPGA_BUILD_INFO "${OPENFPGA_BUILD_INFO}")

message(STATUS "OpenFPGA Version: ${OPENFPGA_VERSION}")

configure_file(${IN_FILE} ${OUT_FILE})
