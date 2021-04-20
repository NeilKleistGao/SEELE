get_filename_component(_path ${CMAKE_CURRENT_LIST_FILE} PATH)
set(RENDERING_FILES "")

file(GLOB _h_files RELATIVE ${_path} ${_path}/*.h)

foreach(_file ${_h_files})
    list(APPEND RENDERING_FILES ${_path}/${_file})
endforeach()

file(GLOB _cc_files RELATIVE ${_path} ${_path}/*.cc)

foreach(_file ${_cc_files})
    list(APPEND RENDERING_FILES ${_path}/${_file})
endforeach()

include(${_path}/basic/CMakeLists.txt)

foreach(_file ${BASIC_RENDERING_FILES})
    list(APPEND RENDERING_FILES ${_file})
endforeach()