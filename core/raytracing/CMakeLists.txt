file(GLOB _files RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)
set(RAYTRACING_LIST )

foreach(_file ${_files})
    list(APPEND RAYTRACING_LIST ${CMAKE_CURRENT_LIST_DIR}/${_file})
endforeach()