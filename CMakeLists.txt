cmake_minimum_required(VERSION 3.15)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

project(OrangeEngine LANGUAGES CXX)

option(ORANGE_ENGINE_BUILD_TESTS "Build tests" OFF)

# Use FetchContent to get vcpkg, so users don't have to get it themselves
include(FetchContent)
FetchContent_Declare(vcpkg
    GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
    GIT_TAG 2022.11.14
)
FetchContent_MakeAvailable(vcpkg)

set(VCPKG_INSTALL_OPTIONS "--no-print-usage")
set(VCPKG_OVERLAY_PORTS ${CMAKE_CURRENT_SOURCE_DIR}/ports)
include("${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake")

add_subdirectory(ext)

include("cmake/StandardProjectSettings.cmake")
include("CMakeDependentOption")
include("cmake/compiler.cmake")

add_subdirectory(src)

if(ORANGE_ENGINE_BUILD_TESTS)
    add_subdirectory(test)
endif()
