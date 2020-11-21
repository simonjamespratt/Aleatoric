target_sources(Aleatoric_Aleatoric
    PRIVATE
        CollectionsProducer.hpp
        DurationsProducer.hpp
        DurationsProducer.cpp
        NumbersProducer.hpp
        NumbersProducer.cpp
)

include(AleatoricHelpers)
manage_headers_for_aleatoric_library()
