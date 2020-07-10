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
    ${CMAKE_SOURCE_DIR}/include/aleatoric/IUniformGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/NumbersProducer.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/NumberProtocol.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Serial.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Basic.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/NoRepetition.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/GroupedRepetition.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/AdjacentSteps.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Periodic.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Walk.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/GranularWalk.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/DiscreteGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/UniformGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/Range.hpp
    ${CMAKE_SOURCE_DIR}/include/aleatoric/NumberProtocolFactory.hpp
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
    Errors/ErrorChecker.hpp
)

add_library(
    AleatoricCompositionTools
    Errors/ErrorChecker.cpp
    Utilities/Utilities.cpp
    Numbers/NumbersProducer.cpp
    Numbers/Serial.cpp
    Numbers/Basic.cpp
    Numbers/NoRepetition.cpp
    Numbers/GroupedRepetition.cpp
    Numbers/AdjacentSteps.cpp
    Numbers/Periodic.cpp
    Numbers/Walk.cpp
    Numbers/GranularWalk.cpp
    Numbers/Range.cpp
    Numbers/UniformGenerator.cpp
    Numbers/DiscreteGenerator.cpp
    Numbers/NumberProtocolFactory.cpp
    Numbers/Cycle.cpp
    Numbers/Precision.cpp
    Numbers/Ratio.cpp
    Numbers/Subset.cpp
    TimeDomain/Prescribed.cpp
    TimeDomain/Multiples.cpp
    TimeDomain/Geometric.cpp
    TimeDomain/DurationsProducer.cpp
    ${HEADER_LIST}
)

target_include_directories(AleatoricCompositionTools
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/aleatoric
        ${pcg_cpp_SOURCE_DIR}/include # TODO: BUILDS-CMAKE: had to make this public in order for thr tests to build. Need to find out why
        Utilities # TODO: BUILDS-CMAKE: again, had to make this public to allow Utilities.hpp to be included in Serial.hpp et al files
    PRIVATE
        Errors
)
