# header-only library

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO martty/VulkanMemoryAllocator
    REF b0c78ced009d8fbe15b2842d78ddb40066d9641b
    SHA512 9cf8cc1658b2b3bd8f36cfcbb7a1f672afb9174878810621a59e7133ca4374e307fe84f38d39ba2950a2e64166a1301dbc4159d0981587bb2f52640881c9e68b
    HEAD_REF vuk
    PATCHES make_it_work.patch
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DVMA_STATIC_VULKAN_FUNCTIONS=ON
        -DVMA_DYNAMIC_VULKAN_FUNCTIONS=OFF
)
vcpkg_cmake_install()
# file(MAKE_DIRECTORY "${CURRENT_PACKAGES_DIR}/include/")
# file(COPY_FILE "${SOURCE_PATH}/include/vk_mem_alloc.h" "${CURRENT_PACKAGES_DIR}/include/vuk_vk_mem_alloc.h")

# file(COPY "${CMAKE_CURRENT_LIST_DIR}/vuk-vma-config.cmake" DESTINATION "${CURRENT_PACKAGES_DIR}/share/vuk-vma")

