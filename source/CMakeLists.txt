FetchContent_Declare(
    pcg-cpp
    GIT_REPOSITORY  https://github.com/imneme/pcg-cpp.git
    GIT_TAG         5b5cac8d61339e810c5dbb4692d868a1d7ca1b2d #latest master commit on 04-02-2020
)
FetchContent_MakeAvailable(pcg-cpp)

# NB: HEADER_LIST is only used for IDE reference purposes and is not a build requirement.
# Also note that is can be globbed, but is potentially a bit flakey
set(HEADER_LIST
    ${aleatoric_SOURCE_DIR}/include/aleatoric/IDiscreteGenerator.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/DiscreteGenerator.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/IUniformGenerator.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/UniformGenerator.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/UniformRealGenerator.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/NumbersProducer.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/NumberProtocol.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/NumberProtocolParameters.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Serial.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Basic.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/NoRepetition.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/GroupedRepetition.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/AdjacentSteps.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Periodic.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Walk.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/GranularWalk.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Range.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Cycle.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Ratio.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Subset.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Precision.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/CollectionsProducer.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/DurationProtocol.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Prescribed.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Multiples.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/Geometric.hpp
    ${aleatoric_SOURCE_DIR}/include/aleatoric/DurationsProducer.hpp

    NumberProtocols/Helpers/CycleStates.hpp
    NumberProtocols/Helpers/SeriesPrinciple.hpp

    Generators/Engine.hpp

    Errors/ErrorChecker.hpp
)

add_library(
    Aleatoric

    DurationProtocols/DurationProtocol.cpp
    DurationProtocols/Geometric.cpp
    DurationProtocols/Multiples.cpp
    DurationProtocols/Prescribed.cpp

    Errors/ErrorChecker.cpp

    Generators/Engine.cpp
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

    NumberProtocols/Helpers/CycleStates.cpp
    NumberProtocols/Helpers/SeriesPrinciple.cpp

    Range.cpp

    ${HEADER_LIST}
)

target_include_directories(Aleatoric
    PUBLIC
        ${aleatoric_SOURCE_DIR}/include/aleatoric
    PRIVATE
        ${pcg-cpp_SOURCE_DIR}/include
        Errors
        NumberProtocols/Helpers
        Generators
)
