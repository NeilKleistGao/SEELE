cmake_minimum_required(VERSION 3.15)
project(SEELE)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(include)

add_definitions(-D PLATFORM_LINUX)

include(core/CMakeLists.txt)
include(components/CMakeLists.txt)
include(assets/CMakeLists.txt)
include(utilities/CMakeLists.txt)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(PLATFORM_PREFIX "/lib/linux")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

add_executable(SEELE main.cc
        ${CORE_LIST}
        ${COMPONENTS_LIST}
        ${ASSETS_LIST}
        ${UTILITIES_LIST})

target_link_libraries(SEELE "${CMAKE_SOURCE_DIR}/${PLATFORM_PREFIX}/liblua52.so" -ldl)

set_target_properties(SEELE PROPERTIES
        COMPILE_FLAGS "-pthread"
        LINK_FLAGS "-pthread")