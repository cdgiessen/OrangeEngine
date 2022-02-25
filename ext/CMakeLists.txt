option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)
option(GLFW_BUILD_EXAMPLES OFF)

option(VUK_LINK_TO_LOADER OFF)

option(OPTIONAL_BUILD_TESTS OFF)
option(EXPECTED_BUILD_TESTS OFF)
option(EXPECTED_BUILD_PACKAGE OFF)

option(FASTNOISE2_NOISETOOL OFF)

find_package(Vulkan REQUIRED)

include(FetchContent)
FetchContent_Declare(
    fc_glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG        3.3.2
)
FetchContent_Declare(
    fc_vkbootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
    GIT_TAG        master
)
FetchContent_Declare(
    fc_imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG        docking
)
FetchContent_Declare(
    fc_optional
    GIT_REPOSITORY https://github.com/TartanLlama/optional
    GIT_TAG        master
)
FetchContent_Declare(
    fc_expected
    GIT_REPOSITORY https://github.com/TartanLlama/expected
    GIT_TAG        master
)
FetchContent_Declare(
    fc_vma
    GIT_REPOSITORY https://github.com/martty/VulkanMemoryAllocator/
    GIT_TAG        b0c78ced009d8fbe15b2842d78ddb40066d9641b
)
FetchContent_Declare(
    fc_nlohmann_json
    GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
    GIT_TAG        main
)
FetchContent_Declare(
    fc_cgltf
    GIT_REPOSITORY https://github.com/jkuhlmann/cgltf
    GIT_TAG        v1.10
)
FetchContent_Declare(
    fc_fastnoise2
    GIT_REPOSITORY https://github.com/Auburn/FastNoise2/
    GIT_TAG        master
)
FetchContent_Declare(
    fc_spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG        v1.x
)
FetchContent_Declare(
    fc_vuk
    GIT_REPOSITORY https://github.com/martty/vuk
    GIT_TAG        v0.3
)
set(CMAKE_POLICY_DEFAULT_CMP0079 NEW)
FetchContent_MakeAvailable(
fc_glfw
fc_vma
fc_vkbootstrap
fc_optional
fc_expected
fc_nlohmann_json
fc_fastnoise2
fc_spdlog
fc_vuk
)

FetchContent_GetProperties(fc_imgui)
if(NOT fc_imgui_POPULATED)
    FetchContent_Populate(fc_imgui)

    add_library(DearImGui STATIC
        ${fc_imgui_SOURCE_DIR}/imgui.cpp
        ${fc_imgui_SOURCE_DIR}/imgui_draw.cpp
        ${fc_imgui_SOURCE_DIR}/imgui_widgets.cpp)
    target_include_directories(DearImGui PUBLIC ${fc_imgui_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
endif()

FetchContent_GetProperties(fc_cgltf)
if(NOT fc_cgltf_POPULATED)
    FetchContent_Populate(fc_cgltf)
    add_library(cgltf STATIC cgltf.cpp)
    target_include_directories(cgltf PUBLIC ${fc_cgltf_SOURCE_DIR})
endif()

add_library(external_dependencies INTERFACE)
target_link_libraries(external_dependencies INTERFACE
    Vulkan::Vulkan
    glfw
    vk-bootstrap
    DearImGui
    tl::optional
    tl::expected
    nlohmann_json::nlohmann_json
    cgltf
    FastNoise
    spdlog::spdlog
    VulkanMemoryAllocator
    vuk
    )


if (ORANGE_ENGINE_BUILD_TESTS)
option(CATCH_BUILD_TESTING OFF)
option(CATCH_ENABLE_WERROR OFF)
option(CATCH_INSTALL_DOCS OFF)
option(CATCH_INSTALL_HELPERS ON)
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1) #remove Catch2 target spam
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG        v3.0.0-preview3
)
FetchContent_MakeAvailable(Catch2)
endif()