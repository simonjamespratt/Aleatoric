# Aleatoric

A static library written in C++ which implements algorithms commonly used in computer-aided music composition. The project is inspired by the library [Real Time Composition Library](http://www.essl.at/works/rtc.html) (RTC-lib) by Karlheinz Essl for use with the [Max](https://cycling74.com/products/max) environment.

The project is still in its infancy and is _not_ production ready. However there is a working release which can be used.

Note that use of the library has only been tested on macOS.

## Basic usage

Download the library (see releases here on github). The download has a `libAleatoric.a` file (`Aleatoric-<version>/lib`) and the required headers (`Aleatoric-<version>/include/aleatoric`).

## Use within CMake projects

Note: The project uses C++ 14.

### As a package using find_package()

Download the library (see releases here on github). The download has a config file (`/Aleatoric-<version>/lib/cmake/Aleatoric/AleatoricConfig.cmake`).

If you place the downloaded files in a location in which cmake automatically searches for packages, then cmake will be able to find it without further direction.

If you place the download elsewhere (such as somewhere within a specific project directory structure) then you will have to add that location to `CMAKE_PREFIX_PATH`.

The [cmake documentation](https://cmake.org/cmake/help/v3.15/command/find_package.html#search-procedure) has more information about the `find_package()` search procedure.

Here is a minimal implementation within a `CMakeLists.txt` file:

```cmake
cmake_minimum_required(VERSION 3.0.0)
project(myProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add this line if downloaded files are not in a location in which cmake automatically searches for packages
list(APPEND CMAKE_PREFIX_PATH /path/to/Aleatoric-<version>)

find_package(Aleatoric)

add_executable(myApp main.cpp)

target_link_libraries(myApp PRIVATE Aleatoric::Aleatoric)
```

### As a subdirectory using FetchContent

When used in this form, there is no need to download the library files. Here is a minimal implementation within a `CMakeLists.txt` file:

```cmake
cmake_minimum_required(VERSION 3.0.0)
project(myProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    Aleatoric
    GIT_REPOSITORY  https://github.com/simonjamespratt/Aleatoric.git
    GIT_TAG         commit-hash-for-a-release-version
)

FetchContent_MakeAvailable(Aleatoric)

add_executable(myApp main.cpp)

target_link_libraries(myApp PRIVATE Aleatoric::Aleatoric)
```

It is recommended that the value for `GIT_TAG` above is a specific commit hash rather than a tag as hashes are more stable.
