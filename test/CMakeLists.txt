add_executable(OrangeEngineTestMath
    math/vector_tests.cpp)

target_link_libraries(OrangeEngineTestMath PRIVATE external_test_dependencies orange_math)
