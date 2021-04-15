cmake_minimum_required(VERSION 3.15)
project(SEELE)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(include)

add_definitions(-D PLATFORM_LINUX)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(PLATFORM_PREFIX "/lib/linux")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

#include(window_manager/CMakeLists.txt)
#include(rendering/CMakeLists.txt)
#include(utils/CMakeLists.txt)
include(script/CMakeLists.txt)
#include(math/CMakeLists.txt)
#include(model/CMakeLists.txt)
#include(include/tga/CMakeLists.txt)
#include(light/CMakeLists.txt)
#include(pipelines/CMakeLists.txt)

add_executable(SEELE main.cc
        ${SCRIPT_FILES})

target_link_libraries(SEELE "${CMAKE_SOURCE_DIR}/${PLATFORM_PREFIX}/liblua52.so" -ldl)

set_target_properties(SEELE PROPERTIES
        COMPILE_FLAGS "-pthread"
        LINK_FLAGS "-pthread")

add_custom_command(TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy_directory "../lua_api" "${EXECUTABLE_OUTPUT_PATH}/lua_api"
        COMMENT "COPY CONFIGURATION FILES")