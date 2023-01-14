
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(ImGui REQUIRED)
find_package(tl-optional REQUIRED)
find_package(tl-expected REQUIRED)
find_package(nlohmann_json REQUIRED)
find_path(CGLTF_INCLUDE_DIRS "cgltf.h")
find_package(spdlog REQUIRED)
find_package(unofficial-vulkan-memory-allocator REQUIRED)

find_package(vk-bootstrap REQUIRED)
find_package(FastNoise2 REQUIRED)

FetchContent_Declare(
    vuk
    GIT_REPOSITORY https://github.com/martty/vuk
    GIT_TAG        be7cf2cab527df1310bfe39a65d72c524fdd2657
)
FetchContent_MakeAvailable(vuk)
add_library(external_dependencies INTERFACE)
target_link_libraries(external_dependencies INTERFACE
    Vulkan::Vulkan
    glfw
    imgui::imgui
    tl::optional
    tl::expected
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    unofficial::vulkan-memory-allocator::vulkan-memory-allocator
    vk-bootstrap::vk-bootstrap
    FastNoise2::FastNoise
    vuk
    )
target_include_directories(external_dependencies INTERFACE ${CGLTF_INCLUDE_DIRS})


if (ORANGE_ENGINE_BUILD_TESTS)
    find_package(Catch2 3 REQUIRED)
    add_library(external_test_dependencies INTERFACE)
    target_link_libraries(external_test_dependencies INTERFACE Catch2::Catch2 Catch2::Catch2WithMain)
endif()
