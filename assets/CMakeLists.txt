file(GLOB _files RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)
set(ASSETS_LIST )

foreach(_file ${_files})
    list(APPEND ASSETS_LIST ${CMAKE_CURRENT_LIST_DIR}/${_file})
endforeach()