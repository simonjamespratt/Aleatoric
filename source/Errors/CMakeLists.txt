target_sources(Aleatoric_Aleatoric
    PRIVATE
        ErrorChecker.hpp
        ErrorChecker.cpp
)

target_include_directories(Aleatoric_Aleatoric
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
