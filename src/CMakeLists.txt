project(gmshparsercpp
    VERSION 1.0
    LANGUAGES CXX
)

add_library(${PROJECT_NAME}
    SHARED
        gmshparsercppMshFile.cpp
)
target_code_coverage(${PROJECT_NAME})

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        fmt::fmt
)

install(
    TARGETS
        gmshparsercpp
    EXPORT
        gmshparsercppTargets
    LIBRARY DESTINATION
        lib
    ARCHIVE DESTINATION
        lib
    RUNTIME DESTINATION
        bin
    INCLUDES DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
        PATTERN "*.h"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    gmshparsercppConfigVersion.cmake
    VERSION
        ${PROJECT_VERSION}
    COMPATIBILITY
        AnyNewerVersion
)

install(
    EXPORT
        gmshparsercppTargets
    FILE
        gmshparsercppTargets.cmake
    NAMESPACE
        gmshparsercpp::
    DESTINATION
        lib/cmake/gmshparsercpp
)

install(
    FILES
        "${CMAKE_SOURCE_DIR}/cmake/gmshparsercppConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/gmshparsercppConfigVersion.cmake"
    DESTINATION
        lib/cmake/gmshparsercpp
)
