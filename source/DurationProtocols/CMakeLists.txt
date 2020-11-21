target_sources(Aleatoric_Aleatoric
    PRIVATE
        DurationProtocol.hpp
        DurationProtocol.cpp
        Geometric.hpp
        Geometric.cpp
        Multiples.hpp
        Multiples.cpp
        Prescribed.hpp
        Prescribed.cpp
)

include(AleatoricHelpers)
manage_headers_for_aleatoric_library()
