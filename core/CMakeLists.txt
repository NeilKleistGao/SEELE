include(${CMAKE_CURRENT_LIST_DIR}/general/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/rasterization/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/raytracing/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/photon/CMakeLists.txt)

set(CORE_LIST ${GENERAL_LIST} ${RASTERIZATION_LIST} ${RAYTRACING_LIST} ${PHOTON_LIST})