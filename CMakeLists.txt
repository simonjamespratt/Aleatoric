# Basic settings
cmake_minimum_required(VERSION 3.14)
project(
    aleatoric
    VERSION 0.1.0
    DESCRIPTION "C++ library providing aleatoric composition tools"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
include(CTest)

add_subdirectory(source)
add_subdirectory(tests)

# docs
find_package(Doxygen REQUIRED
    COMPONENTS dot
    OPTIONAL_COMPONENTS mscgen dia)

if (DOXYGEN_FOUND)
    add_subdirectory(docs)
else()
    message(STATUS "Doxygen not found, not building docs")
endif (DOXYGEN_FOUND)



# Installing and packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
