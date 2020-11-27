# Project-wide settings
cmake_minimum_required(VERSION 3.14)
project(
    aleatoric
    VERSION 0.1.0
    DESCRIPTION "C++ library providing aleatoric composition tools"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies (external content)
add_subdirectory(dependencies)

# Main targets
add_subdirectory(source)
add_subdirectory(docs)

# Tests and Packaging
# Add these targets only if top level project
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(tests)
    add_subdirectory(packaging)
endif()
