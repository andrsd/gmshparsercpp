FLEX_TARGET(MSH_LEXER msh.lex ${CMAKE_CURRENT_BINARY_DIR}/gmshparsercppMshLexer.cpp)

add_library(${PROJECT_NAME}
    SHARED
        gmshparsercppMshFile.cpp
        ${FLEX_MSH_LEXER_OUTPUTS}
)
if(CMAKE_PROJECT_NAME STREQUAL "gmshparsercpp")
target_code_coverage(${PROJECT_NAME})
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/gmshparsercpp>
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${FLEX_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        fmt::fmt
)

install(
    TARGETS gmshparsercpp
    EXPORT gmshparsercppTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(gmshparsercppConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    EXPORT gmshparsercppTargets
    FILE gmshparsercppTargets.cmake
    NAMESPACE gmshparsercpp::
    DESTINATION lib/cmake/gmshparsercpp
)

install(
    FILES
        "${CMAKE_SOURCE_DIR}/cmake/gmshparsercppConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/gmshparsercppConfigVersion.cmake"
    DESTINATION
        lib/cmake/gmshparsercpp
)
