macro(manage_headers_for_aleatoric_library)
    target_include_directories(Aleatoric_Aleatoric
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )

    set_property(
        TARGET Aleatoric_Aleatoric
        APPEND PROPERTY ALEATORIC_PUBLIC_HEADER_DIRS "${CMAKE_CURRENT_LIST_DIR}/")
endmacro()
