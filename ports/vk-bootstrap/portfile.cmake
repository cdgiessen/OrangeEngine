vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO charles-lunarg/vk-bootstrap
    REF 8e61b2d81c3f5f84339735085ff5651f71bbe1e7
    SHA512 2fa3cb0bf51a6e747af92234dfb9a9d6f5003bea002f07ce3ab023f270ccaeed4359ca3402fe2da37f2c93e76e5822dbedc1a1690ca39c39323120092070d226
    HEAD_REF master
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DCMAKE_PROJECT_NAME=
)

vcpkg_cmake_install()
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(COPY "${CMAKE_CURRENT_LIST_DIR}/${PORT}-config.cmake" "${CMAKE_CURRENT_LIST_DIR}/${PORT}-targets-release.cmake" "${CMAKE_CURRENT_LIST_DIR}/${PORT}-targets-debug.cmake" DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT})

configure_file("${SOURCE_PATH}/LICENSE.txt" "${CURRENT_PACKAGES_DIR}/share/${PORT}/copyright" COPYONLY)
