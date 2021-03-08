cmake_minimum_required(VERSION 3.15)
project(SEELE)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_definitions(-D PLATFORM_LINUX)

include(window_manager/CMakeLists.txt)
include(rendering/CMakeLists.txt)

add_executable(SEELE main.cc
        ${WINDOW_MANAGER_HEADERS}
        ${WINDOW_MANAGER_SRCS}
        ${RENDERING_HEADERS}
        ${RENDERING_SRCS})