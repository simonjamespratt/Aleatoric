# TODO: could add the target sources from within the subdirectories if desired

# NB: HEADER_LIST is only used for IDE reference purposes and is not a build requirement.
# Also note that is can be globbed, but is potentially a bit flakey
set(HEADER_LIST
    ${CMAKE_SOURCE_DIR}/include/actlib/IDiscreteGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/IUniformGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/ProducerSteps.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/ProtocolSteps.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/ProducerGranular.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/ProtocolGranular.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/Serial.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/Basic.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/NoRepetition.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/AdjacentSteps.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/Periodic.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/Walk.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/GranularWalk.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/DiscreteGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/UniformGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/Range.hpp
    Engine/Engine.hpp
    Utilities/Utilities.hpp
)

add_library(
    AleatoricCompositionTools
    Engine/Engine.cpp
    Utilities/Utilities.cpp
    Numbers/ProducerSteps.cpp
    Numbers/ProducerGranular.cpp
    Numbers/Serial.cpp
    Numbers/Basic.cpp
    Numbers/NoRepetition.cpp
    Numbers/AdjacentSteps.cpp
    Numbers/Periodic.cpp
    Numbers/Walk.cpp
    Numbers/GranularWalk.cpp
    Numbers/Range.cpp
    Numbers/UniformGenerator.cpp
    Numbers/DiscreteGenerator.cpp
    ${HEADER_LIST}
)

target_include_directories(AleatoricCompositionTools
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/actlib
    PRIVATE
        Engine
        Utilities
)
