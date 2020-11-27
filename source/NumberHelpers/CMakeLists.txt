target_sources(Aleatoric_Aleatoric
    PRIVATE
        CycleStates.hpp
        CycleStates.cpp
        SeriesPrinciple.hpp
        SeriesPrinciple.cpp
)

target_include_directories(Aleatoric_Aleatoric
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
