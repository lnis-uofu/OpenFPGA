cmake_minimum_required(VERSION 3.9)

project("libopenfpgashell")

file(GLOB_RECURSE EXEC_SOURCES test/*.cpp)
file(GLOB_RECURSE LIB_SOURCES src/*/*.cpp)
file(GLOB_RECURSE LIB_HEADERS src/*/*.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

#Remove test executable from library
list(REMOVE_ITEM LIB_SOURCES ${EXEC_SOURCES})

#Create the library
add_library(libopenfpgashell STATIC
             ${LIB_HEADERS}
             ${LIB_SOURCES})
target_include_directories(libopenfpgashell PUBLIC ${LIB_INCLUDE_DIRS})
set_target_properties(libopenfpgashell PROPERTIES PREFIX "") #Avoid extra 'lib' prefix

# Specify link-time dependencies
target_link_libraries(libopenfpgashell
                      libopenfpgautil
                      libvtrutil)

# Decide what readline package to use now 
if (OPENFPGA_READLINE_MODE STREQUAL "libreadline")
  # Include compiler definition and linked lib for libreadline
  target_compile_definitions(libopenfpgashell PRIVATE OPENFPGA_USE_READLINE)
  if(WIN32)
    if (MSVC)
      find_package(unofficial-readline-win32 CONFIG REQUIRED)
      target_link_libraries(libopenfpgashell
                            unofficial::readline-win32::readline)
    else()
      find_package(Readline REQUIRED)
      target_link_libraries(libopenfpgashell
                            readline)
    endif()
  else()
    find_package(Readline REQUIRED)
    target_link_libraries(libopenfpgashell
                          readline)
  endif()
elseif (OPENFPGA_READLINE_MODE STREQUAL "libedit")
  # Include compiler definition and linked lib for libedit
  target_compile_definitions(libopenfpgashell PRIVATE OPENFPGA_USE_LIBEDIT)
  if(WIN32)
      # Windows-specific dependency mapping
      # Note: 'wineditline' or 'libedit' can be installed via: vcpkg install libedit
      find_package(libedit REQUIRED)
      target_link_libraries(libopenfpgashell libedit::libedit)
  elseif(UNIX AND NOT APPLE)
      # Linux-specific PkgConfig mapping
      find_package(PkgConfig REQUIRED)
      pkg_check_modules(EDITLINE REQUIRED libedit)
      target_link_libraries(libopenfpgashell ${EDITLINE_LIBRARIES})
  elseif(APPLE)
      # macOS includes libedit natively in the SDK
      target_link_libraries(libopenfpgashell edit)
  endif()
elseif (OPENFPGA_READLINE_MODE STREQUAL "standard")
  message(STATUS "Use the standard minimum readline mode in shell interface. No readline features will be included")
else()
  # This acts as a fallback safety net
  message(FATAL_ERROR "Unknown readline feature mode: ${OPENFPGA_READLINE_MODE}")
endif()

# Create the test executable
foreach(testsourcefile ${EXEC_SOURCES})
    # Use a simple string replace, to cut off .cpp.
    get_filename_component(testname ${testsourcefile} NAME_WE)
    add_executable(${testname} ${testsourcefile})
    # Make sure the library is linked to each test executable
    target_link_libraries(${testname} libopenfpgashell)
endforeach(testsourcefile ${EXEC_SOURCES})

install(TARGETS libopenfpgashell
        DESTINATION bin
        COMPONENT openfpga_package
)
