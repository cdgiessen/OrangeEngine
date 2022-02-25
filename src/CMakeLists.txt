set(ORANGE_ENGINE_SOURCE_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(math)
add_subdirectory(core)
add_subdirectory(render)


add_executable(main main.cpp)
target_link_libraries(main PUBLIC project_dependencies
    orange_math orange_core orange_renderer)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
