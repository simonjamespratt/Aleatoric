FetchContent_Declare(
    catch2
    GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
    GIT_TAG         v2.11.0
)
FetchContent_MakeAvailable(catch2)

FetchContent_Declare(
    trompeloeil
    GIT_REPOSITORY  https://github.com/rollbear/trompeloeil.git
    GIT_TAG         v35
)
FetchContent_MakeAvailable(trompeloeil)


add_executable(Tests
    main.cpp
    # EngineTest.cpp
    DiscreteGeneratorTest.cpp
    UniformGeneratorTest.cpp
    SerialTest.cpp
)

target_link_libraries(Tests
    PRIVATE
    AleatoricCompositionTools
    Catch2::Catch2
    trompeloeil
)

# TODO: There are probably more elegant ways to get the tests to have access to the library's private includes
# See: https://stackoverflow.com/questions/54652349/how-to-unit-test-private-features-of-library-tdd-with-cmake
target_include_directories(Tests PRIVATE
    ${CMAKE_SOURCE_DIR}/source/Numbers
    ${CMAKE_SOURCE_DIR}/source/Engine
)
