add_library(Aleatoric_Aleatoric STATIC)

add_library(Aleatoric::Aleatoric ALIAS Aleatoric_Aleatoric)

set_target_properties(Aleatoric_Aleatoric
    PROPERTIES
        OUTPUT_NAME Aleatoric
        EXPORT_NAME Aleatoric
)

# gather paths for all subdirectories that contain public headers
# used to copy the headers during install
set_property(TARGET Aleatoric_Aleatoric PROPERTY ALEATORIC_PUBLIC_HEADER_DIRS)

add_subdirectory(DurationProtocols)
add_subdirectory(Engine)
add_subdirectory(Errors)
add_subdirectory(Generators)
add_subdirectory(NumberHelpers)
add_subdirectory(NumberProtocols)
add_subdirectory(Producers)
add_subdirectory(Range)

# build interface gen expr required here for reasons:
# https://gitlab.kitware.com/cmake/cmake/-/issues/17357
# https://gitlab.kitware.com/cmake/cmake/-/issues/15415#note_333595
# https://gitlab.kitware.com/cmake/cmake/-/issues/15415#note_634114
# bottom link is the solution used here
target_link_libraries(Aleatoric_Aleatoric PRIVATE "$<BUILD_INTERFACE:pcgCppLib>")
