# TODO: BUILDS-CMAKE: could add the target sources from within the subdirectories if desired

FetchContent_Declare(
    pcg_cpp
    GIT_REPOSITORY  https://github.com/imneme/pcg-cpp.git
    GIT_TAG         origin/master
)
FetchContent_MakeAvailable(pcg_cpp)

# NB: HEADER_LIST is only used for IDE reference purposes and is not a build requirement.
# Also note that is can be globbed, but is potentially a bit flakey
set(HEADER_LIST
    ${CMAKE_SOURCE_DIR}/include/aleatoric/IDiscreteGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/DiscreteGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/IUniformGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/UniformGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/UniformRealGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/NumbersProducer.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/NumberProtocol.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/NumberProtocolParameters.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Serial.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Basic.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/NoRepetition.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/GroupedRepetition.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/AdjacentSteps.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Periodic.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Walk.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/GranularWalk.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Range.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Cycle.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Ratio.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Subset.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Precision.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/CollectionsProducer.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/DurationProtocol.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Prescribed.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Multiples.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Geometric.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/DurationsProducer.hpp
    Utilities/Utilities.hpp
    Utilities/CycleStates.hpp
    Errors/ErrorChecker.hpp
)

add_library(
    Aleatoric

    DurationProtocols/DurationProtocol.cpp
    DurationProtocols/Geometric.cpp
    DurationProtocols/Multiples.cpp
    DurationProtocols/Prescribed.cpp

    Errors/ErrorChecker.cpp

    Generators/DiscreteGenerator.cpp
    Generators/UniformGenerator.cpp
    Generators/UniformRealGenerator.cpp

    NumberProtocols/AdjacentSteps.cpp
    NumberProtocols/Basic.cpp
    NumberProtocols/Cycle.cpp
    NumberProtocols/GranularWalk.cpp
    NumberProtocols/GroupedRepetition.cpp
    NumberProtocols/NoRepetition.cpp
    NumberProtocols/NumberProtocol.cpp
    NumberProtocols/NumberProtocolParameters.cpp
    NumberProtocols/Periodic.cpp
    NumberProtocols/Precision.cpp
    NumberProtocols/Ratio.cpp
    NumberProtocols/Serial.cpp
    NumberProtocols/Subset.cpp
    NumberProtocols/Walk.cpp

    Producers/DurationsProducer.cpp
    Producers/NumbersProducer.cpp

    Utilities/Utilities.cpp
    Utilities/CycleStates.cpp

    Range.cpp

    ${HEADER_LIST}
)

target_include_directories(Aleatoric
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/aleatoric
        ${pcg_cpp_SOURCE_DIR}/include # TODO: BUILDS-CMAKE: had to make this public in order for thr tests to build. Need to find out why
        Utilities # TODO: BUILDS-CMAKE: again, had to make this public to allow Utilities.hpp to be included in Serial.hpp et al files
    PRIVATE
        Errors
)
