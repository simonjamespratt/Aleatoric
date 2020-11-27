target_sources(Aleatoric_Aleatoric
    PRIVATE
        CollectionsProducer.hpp
        DurationsProducer.hpp
        DurationsProducer.cpp
        NumbersProducer.hpp
        NumbersProducer.cpp
)

target_include_directories(Aleatoric_Aleatoric
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
