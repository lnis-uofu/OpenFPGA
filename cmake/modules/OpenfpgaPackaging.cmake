# Installer creation
set(CPACK_PACKAGE_NAME ${PROJECT_NAME} CACHE STRING "The resulting package name")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenFPGA design tool suites" CACHE STRING "OpenFPGA design tool suites")
set(CPACK_PACKAGE_VENDOR "RaFACT")
set(CPACK_PACKAGE_CONTACT "xifan.tang@ieee.org")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "RaFACT")
# TODO: use CMAKE to parse existing dependency file, e.g., .github/workflow/regtest_dependency.sh
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libreadline, libstdc++6, iverilog, libffi-dev, libgcc1, libc6")
set(CPACK_PACKAGE_VERSION ${VERSION_NUMBER})
set(CPACK_PACKAGE_VERSION_MAJOR ${OPENFPGA_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OPENFPGA_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OPENFPGA_VERSION_PATCH})

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

# Try to improve the runtime, use as many cores as possible
set(CPACK_THREADS "0")
# we are interested in cross-platform support. IFW is the only generator that allows for it, so set it explicitly
if (OPENFPGA_GUI_INSTALLER) 
  set(CPACK_GENERATOR "IFW")
else()
  set(CPACK_GENERATOR DEB)
endif()

if (CPACK_GENERATOR STREQUAL "IFW")
  # set some IFW specific variables, which can be derived from the more generic variables given above
  set(CPACK_IFW_VERBOSE ON)
  set(CPACK_IFW_PACKAGE_TITLE ${CPACK_PACKAGE_NAME})
  set(CPACK_IFW_PACKAGE_PUBLISHER ${CPACK_PACKAGE_VENDOR})
  set(CPACK_IFW_PRODUCT_URL "https://github.com/lnis-uofu/OpenFPGA")
  
  # create a more memorable name for the maintenance tool (used for uninstalling the package)
  set(CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_NAME ${PROJECT_NAME}_MaintenanceTool)
  set(CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_INI_FILE ${CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_NAME}.ini)

  # customise the theme if required
  set(CPACK_IFW_PACKAGE_WIZARD_STYLE "Modern")
  
  # adjust the default size
  set(CPACK_IFW_PACKAGE_WIZARD_DEFAULT_HEIGHT 400)
  
  # set the installer icon and logo
  if(WIN32)
    set(CPACK_IFW_PACKAGE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/docs/source/overview/figures/OpenFPGA_logo.ico)
    elseif(APPLE)
    set(CPACK_IFW_PACKAGE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/docs/source/overview/figures/OpenFPGA_logo.icns)
  endif()
  set(CPACK_IFW_PACKAGE_WINDOWS_LOGO ${CMAKE_CURRENT_SOURCE_DIR}/docs/source/overview/figures/OpenFPGA_logo.png)
  set(CPACK_IFW_PACKAGE_LOGO ${CMAKE_CURRENT_SOURCE_DIR}/docs/source/overview/figures/OpenFPGA_logo.png)
endif() 

# Variables must be defined before including the CPACK module
include(CPack)
include(CPackIFW)

cpack_add_component(Unspecified
  REQUIRED
  DISPLAY_NAME "OpenFPGA Tools"
  DESCRIPTION "OpenFPGA Foundational Tools"
)

cpack_add_component(openfpga_package
  REQUIRED
  DISPLAY_NAME "OpenFPGA Engine"
  DESCRIPTION "Main engine for OpenFPGA Tool Suites"
)

if (OPENFPGA_INSTALL_DOC)
  message("Include documentation in installer")
  cpack_add_component(openfpga_doc_package
    DISPLAY_NAME "Documentation"
    DESCRIPTION "Documentation and Tutorials"
  )
endif()

if (CPACK_GENERATOR STREQUAL "IFW")
  cpack_ifw_configure_component(Unspecified
    DISPLAY_NAME "OpenFPGA libraries"
    REQUIRED
    SORTING_PRIORITY 3
    LICENSES "License" ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE 
  )

  cpack_ifw_configure_component(openfpga_package
    REQUIRED
    SORTING_PRIORITY 2
    LICENSES "License" ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE 
  )

  if (OPENFPGA_INSTALL_DOC)
    message("Include documentation in GUI installer")
    cpack_ifw_configure_component(openfpga_doc_package
      REQUIRED
      SORTING_PRIORITY 0
      LICENSES "License" ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE 
    )
  endif()
endif() 
