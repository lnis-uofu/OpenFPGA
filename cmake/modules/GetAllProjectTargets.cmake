# 1. Define a macro to recursively collect all targets in the project
macro(get_all_project_targets out_list current_dir)
    # Get targets in the current directory scope
    get_directory_property(local_targets DIRECTORY "${current_dir}" BUILDSYSTEM_TARGETS)
    list(APPEND ${out_list} ${local_targets})
    
    # Recurse into subdirectories
    get_directory_property(subdirs DIRECTORY "${current_dir}" SUBDIRECTORIES)
    foreach(subdir IN LISTS subdirs)
        get_all_project_targets(${out_list} "${subdir}")
    endforeach()
endmacro()
