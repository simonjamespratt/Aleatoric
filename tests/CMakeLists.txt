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
    Aleatoric_Aleatoric
    Catch2::Catch2
    trompeloeil
)

target_include_directories(Tests PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Mocks
)

# Populate CMAKE_MODULE_PATH with Catch helper scripts
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
include(Catch)
catch_discover_tests(Tests)
