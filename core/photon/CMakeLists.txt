file(GLOB _files RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)
set(PHOTON_LIST )

foreach(_file ${_files})
    list(APPEND PHOTON_LIST ${CMAKE_CURRENT_LIST_DIR}/${_file})
endforeach()