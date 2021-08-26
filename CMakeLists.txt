cmake_minimum_required(VERSION 3.15)
project(OrangeEngine LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

include("cmake/StandardProjectSettings.cmake")
include("CMakeDependentOption")
include("cmake/compiler.cmake")

add_subdirectory(ext)

option(ORANGE_ENGINE_BUILD_TESTS "Build tests" ON)

add_library(project_dependencies INTERFACE)
target_link_libraries(project_dependencies INTERFACE external_dependencies)
add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND ORANGE_ENGINE_BUILD_TESTS)
    add_subdirectory(test)
endif()