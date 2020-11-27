#Installing
# NB: Run this command in terminal to install to a temp staging directory. Useful for seeing the output of installing.
# cmake --install /absolute/path/to/Aleatoric/build --prefix /absolute/path/to/a/staging/directory --component <optional e.g. AleatoricLibrary>
# Also see: https://cmake.org/cmake/help/latest/manual/cmake.1.html#install-a-project

# Base install location is default value of CMAKE_INSTALL_PREFIX, which on unix systems, is /usr/local
# Note however, that CMAKE_INSTALL_PREFIX is irrelevant when packaging as basic archive

# Relative directory structure layout:
include(GNUInstallDirs)

set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME})

install(
    TARGETS Aleatoric_Aleatoric
    EXPORT Aleatoric
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT AleatoricLibrary
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aleatoric
)

get_property(aleatoricPublicHeaders TARGET Aleatoric_Aleatoric PROPERTY ALEATORIC_PUBLIC_HEADER_DIRS)

install(
    DIRECTORY ${aleatoricPublicHeaders}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aleatoric
    COMPONENT AleatoricLibrary
    FILES_MATCHING PATTERN *.hpp
)

# Install for export set including Config.cmake for use with find_package()
# in other cmake projects
install(
    EXPORT Aleatoric
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Aleatoric
    COMPONENT AleatoricLibrary
    NAMESPACE Aleatoric::
    FILE AleatoricConfig.cmake
)

# Packaging
# NB: BUILD PACKAGE: ONLY DO THIS WHEN BUILD TYPE IS RELEASE
# From the root of the project, run this command to build the package target:
# cmake --build ./build --target package
# cmake tools doesn't expose the package target so cannot build this target through vscode
# for further info:
# https://cmake.org/cmake/help/latest/module/CPack.html
# https://cmake.org/cmake/help/latest/manual/cmake.1.html#build-a-project

set(CPACK_PACKAGE_NAME Aleatoric)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}")

if(WIN32)
    set(CPACK_GENERATOR ZIP)
elseif(APPLE)
    set(CPACK_GENERATOR TGZ)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR TGZ)
else()
    set(CPACK_GENERATOR TGZ)
endif()

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)
set(CPACK_COMPONENTS_ALL AleatoricLibrary)
set(CPACK_COMPONENT_INCLUDE_TOPLEVEL_DIRECTORY OFF)

include(CPack)

cpack_add_component(AleatoricLibrary)
