set(ORANGE_ENGINE_SOURCE_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(math)
add_subdirectory(core)
add_subdirectory(render)


add_executable(temp_main main.cpp)
target_link_libraries(temp_main PUBLIC project_dependencies
    orange_math orange_core)
set_target_properties(temp_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
