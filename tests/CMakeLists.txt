FetchContent_Declare(
    Catch2
    GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
    GIT_TAG         ff349a50bfc6214b4081f4ca63c7de35e2162f60 #v2.13.3
)
# TODO: CMAKE: this method can take a list so call it once with both catch2 and tromp...)
FetchContent_MakeAvailable(Catch2)

# NB: Having run FetchContent for catch, the CMAKE_MODULE_PATH is not populated with the helper scripts
# Add them here
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
include(Catch)

FetchContent_Declare(
    trompeloeil
    GIT_REPOSITORY  https://github.com/rollbear/trompeloeil.git
    GIT_TAG         0b8b66cfdb0d0a10fbd0d9268917eb1bf858aaf6 #v39
)
FetchContent_MakeAvailable(trompeloeil)

add_executable(Tests
    main.cpp
    DiscreteGeneratorTest.cpp
    UniformGeneratorTest.cpp
    UniformRealGeneratorTest.cpp
    SerialTest.cpp
    BasicTest.cpp
    NoRepetitionTest.cpp
    AdjacentStepsTest.cpp
    PeriodicTest.cpp
    WalkTest.cpp
    GranularWalkTest.cpp
    NumbersProducerTest.cpp
    CollectionsProducerTest.cpp
    DurationsProducerTest.cpp
    CycleTest.cpp
    PrecisionTest.cpp
    PrescribedTest.cpp
    MultiplesTest.cpp
    GeometricTest.cpp
    RatioTest.cpp
    GroupedRepetitionTest.cpp
    SubsetTest.cpp
    RangeTest.cpp
)

target_link_libraries(Tests
    PRIVATE
    Aleatoric
    Catch2::Catch2
    trompeloeil
)

target_include_directories(Tests PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Mocks
)

catch_discover_tests(Tests)
