# TODO: could add the target sources from within the subdirectories if desired
add_library(
    AleatoricCompositionTools
    Engine/Engine.cpp
    Numbers/Producer.cpp
    Numbers/Serial.cpp
    Numbers/Range.cpp
    Numbers/UniformGenerator.cpp
    Numbers/DiscreteGenerator.cpp
)

target_include_directories(AleatoricCompositionTools
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/actlib
    PRIVATE
        Engine
        Numbers
)
