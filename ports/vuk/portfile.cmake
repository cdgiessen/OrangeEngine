vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO martty/vuk
    REF be7cf2cab527df1310bfe39a65d72c524fdd2657
    SHA512 1417b0f70bac941f7bd932a1d7f1a0ccb9b1ddd401104caef02af4ae9bed7b1071afd210ec3a5c8d5cf276bab20f6914ebef277d1ef5c00e500f8f01797e63ea
    HEAD_REF master
    PATCHES use_find_package.patch
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DVUK_USE_VULKAN_SDK=ON
        -DVUK_USE_SHADERC=OFF
        -DVUK_USE_DXC=OFF
)
vcpkg_cmake_install()
vcpkg_cmake_config_fixup()

