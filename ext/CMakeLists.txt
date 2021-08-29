option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)
option(GLFW_BUILD_EXAMPLES OFF)

option(VUK_LINK_TO_LOADER OFF)

option(OPTIONAL_BUILD_TESTS OFF)
option(EXPECTED_BUILD_TESTS OFF)
option(EXPECTED_BUILD_PACKAGE OFF)

option(FASTNOISE2_NOISETOOL OFF)

include(FetchContent)
FetchContent_Declare(
    fc_glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG        3.3.2
)
FetchContent_Declare(
        fc_vk_module
        GIT_REPOSITORY https://github.com/cdgiessen/vk-module
        GIT_TAG        master
    )
FetchContent_Declare(
    fc_vkbootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
    GIT_TAG        swapchain
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
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
    GIT_TAG        v2.3.0
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
    fc_entt
    GIT_REPOSITORY https://github.com/skypjack/entt
    GIT_TAG        v3.7.1
)
FetchContent_Declare(
    fc_moodycamelqueue
    GIT_REPOSITORY https://github.com/cameron314/concurrentqueue
    GIT_TAG        master
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

FetchContent_MakeAvailable(
fc_glfw
fc_vk_module
fc_vkbootstrap
fc_optional fc_expected
fc_nlohmann_json
fc_entt
fc_moodycamelqueue
fc_fastnoise2
fc_spdlog
)

target_link_libraries( vk-bootstrap PUBLIC vulkan-simple-cpp)

FetchContent_GetProperties(fc_imgui)
if(NOT fc_imgui_POPULATED)
    FetchContent_Populate(fc_imgui)

    add_library(DearImGui STATIC
        ${fc_imgui_SOURCE_DIR}/imgui.cpp
        ${fc_imgui_SOURCE_DIR}/imgui_draw.cpp
        ${fc_imgui_SOURCE_DIR}/imgui_widgets.cpp)
    target_include_directories(DearImGui PUBLIC ${fc_imgui_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
endif()

get_target_property(VK_MODULE_HEADER_INCLUDE_DIR vulkan-simple-cpp INCLUDE_DIRECTORIES)

FetchContent_GetProperties(fc_vma)
if(NOT fc_vma_POPULATED)
    FetchContent_Populate(fc_vma)
    add_library(VulkanMemoryAllocator INTERFACE)

    target_include_directories(VulkanMemoryAllocator INTERFACE ${fc_vma_SOURCE_DIR}/include ${VK_MODULE_HEADER_INCLUDE_DIR})
    target_compile_definitions(VulkanMemoryAllocator INTERFACE VMA_DYNAMIC_VULKAN_FUNCTIONS=false)
endif()

FetchContent_GetProperties(fc_cgltf)
if(NOT fc_cgltf_POPULATED)
    FetchContent_Populate(fc_cgltf)
    add_library(cgltf STATIC cgltf.cpp)
    target_include_directories(cgltf PUBLIC ${fc_cgltf_SOURCE_DIR})
endif()

add_library(external_dependencies INTERFACE)
target_link_libraries(external_dependencies INTERFACE
    glfw
    vulkan-simple-cpp
    vk-bootstrap
    DearImGui
    tl::optional tl::expected
    nlohmann_json::nlohmann_json
    cgltf
    EnTT::EnTT
    concurrentqueue
    FastNoise
    spdlog::spdlog
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