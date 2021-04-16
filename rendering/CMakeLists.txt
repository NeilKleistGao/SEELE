get_filename_component(_path ${CMAKE_CURRENT_LIST_FILE} PATH)
file(GLOB _files RELATIVE ${_path} ${_path}/*)
set(RENDERING_FILES "")

foreach(_file ${_files})
    list(APPEND RENDERING_FILES ${_path}/${_file})
endforeach()