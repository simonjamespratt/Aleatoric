# TODO: could add the target sources from within the subdirectories if desired

# NB: HEADER_LIST is only used for IDE reference purposes and is not a build requirement.
# Also note that is can be globbed, but is potentially a bit flakey
set(HEADER_LIST
    ${CMAKE_SOURCE_DIR}/include/actlib/IDiscreteGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/IUniformGenerator.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/Producer.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/ProtocolInterface.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/Serial.hpp
    ${CMAKE_SOURCE_DIR}/include/actlib/Basic.hpp
    Engine/Engine.hpp
    Numbers/DiscreteGenerator.hpp
    Numbers/Range.hpp
    Numbers/UniformGenerator.hpp
)

add_library(
    AleatoricCompositionTools
    Engine/Engine.cpp
    Numbers/Producer.cpp
    Numbers/Serial.cpp
    Numbers/Basic.cpp
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
        Numbers
)
