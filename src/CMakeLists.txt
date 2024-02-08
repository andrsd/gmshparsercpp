add_library(${PROJECT_NAME}
    ${GMSHPARSERCPP_LIBRARY_TYPE}
        MshFile.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/MshLexer.cpp
)

if(CMAKE_PROJECT_NAME STREQUAL "gmshparsercpp")
    target_code_coverage(${PROJECT_NAME})
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        fmt::fmt
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/MshLexer.cpp
    COMMAND ${FLEX_EXECUTABLE} -o ${CMAKE_CURRENT_BINARY_DIR}/MshLexer.cpp ${PROJECT_SOURCE_DIR}/src/msh.lex
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
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
        "${PROJECT_SOURCE_DIR}/cmake/gmshparsercppConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/gmshparsercppConfigVersion.cmake"
    DESTINATION
        lib/cmake/gmshparsercpp
)
