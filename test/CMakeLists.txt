add_executable(OrangeEngineTestMath
    math/vector_tests.cpp)

target_link_libraries(OrangeEngineTestMath PRIVATE Catch2::Catch2WithMain orange_math)