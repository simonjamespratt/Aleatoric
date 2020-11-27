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

target_include_directories(Aleatoric_Aleatoric
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
