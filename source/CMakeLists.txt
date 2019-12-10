# TODO: could add the target sources from within the subdirectories if desired
add_library(
    AleatoricCompositionTools
    NumberGenerators.cpp
    Numbers/UniformGenerator.cpp
    Numbers/DiscreteGenerator.cpp
    Engine/Engine.cpp
)

target_include_directories(AleatoricCompositionTools
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/actlib
    PRIVATE
        Engine
        Numbers
)
