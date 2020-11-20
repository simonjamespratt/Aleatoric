add_library(Aleatoric_Aleatoric STATIC)

add_library(Aleatoric::Aleatoric ALIAS Aleatoric_Aleatoric)

set_target_properties(Aleatoric_Aleatoric
    PROPERTIES
        OUTPUT_NAME Aleatoric
        EXPORT_NAME Aleatoric
)

add_subdirectory(DurationProtocols)
add_subdirectory(Engine)
add_subdirectory(Errors)
add_subdirectory(Generators)
add_subdirectory(NumberHelpers)
add_subdirectory(NumberProtocols)
add_subdirectory(Producers)
add_subdirectory(Range)

target_link_libraries(Aleatoric_Aleatoric PRIVATE pcgCppLib)
