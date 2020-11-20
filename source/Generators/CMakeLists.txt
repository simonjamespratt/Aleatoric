target_sources(Aleatoric_Aleatoric
    PRIVATE
        IDiscreteGenerator.hpp
        DiscreteGenerator.hpp
        DiscreteGenerator.cpp

        IUniformGenerator.hpp
        UniformGenerator.hpp
        UniformGenerator.cpp

        UniformRealGenerator.hpp
        UniformRealGenerator.cpp
)

target_include_directories(Aleatoric_Aleatoric
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
