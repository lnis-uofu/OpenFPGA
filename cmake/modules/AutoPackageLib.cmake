# Automatically follow the symbolic link to an .so file
# and add it to installer. Good for collecting dependencies
function(auto_package_lib file_list)
  foreach (src ${file_list})
      if(IS_SYMLINK ${src})
          file(REAL_PATH ${src} target)
          message("Follow the real path to .so file: ${src} ->  ${target}")
          install(FILES ${target}
              DESTINATION ${CMAKE_INSTALL_LIBDIR}
              COMPONENT dep_package
          )
      endif()
      install(FILES ${file_list}
          DESTINATION ${CMAKE_INSTALL_LIBDIR}
          COMPONENT dep_package
      )
endforeach()

endfunction(auto_package_lib)
