
find_package(Vulkan REQUIRED GLOBAL)
find_package(glfw3 REQUIRED GLOBAL)
find_package(ImGui REQUIRED GLOBAL)
find_package(tl-optional REQUIRED GLOBAL)
find_package(tl-expected REQUIRED GLOBAL)
find_package(nlohmann_json REQUIRED GLOBAL)
find_path(CGLTF_INCLUDE_DIRS "cgltf.h")
find_package(spdlog REQUIRED GLOBAL)
find_package(unofficial-vulkan-memory-allocator REQUIRED GLOBAL)

find_package(vk-bootstrap REQUIRED GLOBAL)
find_package(FastNoise2 REQUIRED GLOBAL)

# needed for VUK
find_package(unofficial-concurrentqueue CONFIG REQUIRED GLOBAL)
find_package(doctest CONFIG REQUIRED GLOBAL)
find_package(fmt CONFIG REQUIRED GLOBAL)
find_package(imgui CONFIG REQUIRED GLOBAL)
find_package(robin_hood CONFIG REQUIRED GLOBAL)
find_package(spirv_cross_core REQUIRED GLOBAL)

find_package(vuk CONFIG REQUIRED GLOBAL)

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
    vuk::vuk
    )
target_include_directories(external_dependencies INTERFACE ${CGLTF_INCLUDE_DIRS})


if (ORANGE_ENGINE_BUILD_TESTS)
    find_package(Catch2 3 REQUIRED GLOBAL)
endif()
