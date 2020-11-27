include(FetchContent)

FetchContent_Declare(
    pcg-cpp
    GIT_REPOSITORY  https://github.com/imneme/pcg-cpp.git
    GIT_TAG         5b5cac8d61339e810c5dbb4692d868a1d7ca1b2d #latest master commit on 04-02-2020
)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
    GIT_TAG         ff349a50bfc6214b4081f4ca63c7de35e2162f60 #v2.13.3
)

FetchContent_Declare(
    trompeloeil
    GIT_REPOSITORY  https://github.com/rollbear/trompeloeil.git
    GIT_TAG         0b8b66cfdb0d0a10fbd0d9268917eb1bf858aaf6 #v39
)

FetchContent_MakeAvailable(pcg-cpp Catch2 trompeloeil)

add_library(pcgCppLib INTERFACE)
target_include_directories(pcgCppLib INTERFACE ${pcg-cpp_SOURCE_DIR}/include)
