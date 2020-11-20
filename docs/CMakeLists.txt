find_package(Doxygen REQUIRED
    COMPONENTS dot
    OPTIONAL_COMPONENTS mscgen dia)

if (DOXYGEN_FOUND)
    set(DOXYGEN_USE_MATHJAX YES)
    doxygen_add_docs(docs
        # following line won't work now after headers have been incorporated into source directories
        ${PROJECT_SOURCE_DIR}/include/aleatoric/
        general.md
        COMMENT "Generating doc files"
    )
else()
    message(STATUS "Doxygen not found, not building docs")
endif (DOXYGEN_FOUND)
