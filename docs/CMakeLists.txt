set(DOXYGEN_USE_MATHJAX YES)

doxygen_add_docs(docs
    ${PROJECT_SOURCE_DIR}/include/aleatoric/
    general.md
    COMMENT "Generating doc files"
)
