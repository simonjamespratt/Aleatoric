# TODO: Testing with catch can be more elegantly incorporated.
# See: https://cliutils.gitlab.io/modern-cmake/chapters/testing/catch.html
add_executable(Tests
    main.cpp
    # EngineTest.cpp
    DiscreteGeneratorTest.cpp
    UniformGeneratorTest.cpp
    SerialTest.cpp
)

target_link_libraries(Tests PUBLIC AleatoricCompositionTools)

# TODO: There are probably more elegant ways to get the tests to have access to the library's private includes
# See: https://stackoverflow.com/questions/54652349/how-to-unit-test-private-features-of-library-tdd-with-cmake
target_include_directories(Tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/source/Numbers
    ${CMAKE_SOURCE_DIR}/source/Engine
)
