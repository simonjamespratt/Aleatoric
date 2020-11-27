# Project-wide settings
cmake_minimum_required(VERSION 3.15)
project(
    aleatoric
    VERSION 0.1.0
    DESCRIPTION "C++ library providing aleatoric composition tools"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Helpers
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Dependencies (external content)
add_subdirectory(dependencies)

# Main targets
add_subdirectory(source)

# docs are removed as a target until they are up to date
# add_subdirectory(docs)

# Tests and Packaging
# Add these targets only if top level project
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # NB: enable_testing() needs to be done AT THE TOP LEVEL.
    # From  Craig Scott. “Professional CMake: A Practical Guide”:
    # “The effect of this function is to direct CMake to write out a CTest input file in the CMAKE_CURRENT_BINARY_DIR with details
    # of all the tests defined in the project (more accurately, those tests defined in the current directory scope and below).
    # enable_testing() can be called in a subdirectory without error, but without a call to enable_testing() at the top level,
    # the CTest input file will not be created at the top of the build tree, which is where it is normally expected to be.”

    # NB: Additionally, don't use include(CTest) unless necessary because it adds a load of additional Utility targets
    # (that aren't useful unless using CTest in conjunction with CDash, I think!)
    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(packaging)
endif()
