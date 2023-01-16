vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO Auburn/FastNoise2
    REF 11072bc0dbe4f41ea87839dfed369ddfe7c3864a
    SHA512 0382e2e71a539e8f0a32f92202ea8dfcaebc6ca26baac876afaeced411be2917614f39bbfde1475f5d88d90b3124db32ffb49fb42a310d06ba6b4c8a91a5a9dc
    HEAD_REF v0.9.7
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DFASTNOISE2_NOISETOOL=OFF
)
vcpkg_cmake_install()

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME "copyright")
