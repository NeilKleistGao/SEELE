cmake_minimum_required(VERSION 3.15)
project(SEELE)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(include)

add_definitions(-D PLATFORM_LINUX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(PLATFORM_PREFIX "/lib/linux")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

include(window_manager/CMakeLists.txt)
include(rendering/CMakeLists.txt)
include(utils/CMakeLists.txt)

add_executable(SEELE main.cc
        ${WINDOW_MANAGER_HEADERS}
        ${WINDOW_MANAGER_SRCS}
        ${RENDERING_HEADERS}
        ${RENDERING_SRCS}
        ${UTILS_HEADERS}
        ${UTILS_SRCS})

target_link_libraries(SEELE "${CMAKE_SOURCE_DIR}/${PLATFORM_PREFIX}/libX11.so")
target_link_libraries(SEELE "${CMAKE_SOURCE_DIR}/${PLATFORM_PREFIX}/libXrand.so")
target_link_libraries(SEELE "${CMAKE_SOURCE_DIR}/${PLATFORM_PREFIX}/libXi.so")
target_link_libraries(SEELE "${CMAKE_SOURCE_DIR}/${PLATFORM_PREFIX}/libXi.a")

add_custom_command(TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy_directory "../configurations" "${EXECUTABLE_OUTPUT_PATH}/configurations"
        COMMENT "COPY CONFIGURATION FILES")