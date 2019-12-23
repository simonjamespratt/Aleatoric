# TODO: Further improvements can be made to this build.
# See: https://gitlab.com/CLIUtils/modern-cmake/tree/master/examples/extended-project for a full project
# which is based the principles here: https://cliutils.gitlab.io/modern-cmake/

# Basic settings
cmake_minimum_required(VERSION 3.14)
project(
    actlib
    VERSION 0.1.0
    DESCRIPTION "Static C++ library providing aleatoric composition tools"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# target_compile_features(calclib PUBLIC cxx_std_11)
# this is another way to do this, but specifically for a particular target

include(FetchContent)
include(CTest)

add_subdirectory(source)
add_subdirectory(tests)

# Misc
# TODO: what is the purpose of this?
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
