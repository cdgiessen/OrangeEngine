vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO martty/vuk
    REF 387cf1628a2361049483866acd229433b3b70456
    SHA512 d448079e46327aab68b64524f866b8872f89d2385d8f220d0c36a87197c03d388ce5185c186ff076e9f708d94c1fc507f719ddff8714ad244e1cd7f828c4dae9
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

