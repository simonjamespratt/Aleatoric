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

include(AleatoricHelpers)
manage_headers_for_aleatoric_library()
