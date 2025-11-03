add_library(${PROJECT_NAME}
    ${GMSHPARSERCPP_LIBRARY_TYPE}
        Exception.cpp
        MshFile.cpp
        MshLexer.cpp
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

if (GMSHPARSERCPP_WITH_FMT)
    target_compile_definitions(${PROJECT_NAME} PUBLIC GMSHPARSERCPP_WITH_FMT)
    target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
endif()

if (GMSHPARSERCPP_INSTALL)
    install(
        TARGETS gmshparsercpp
        EXPORT gmshparsercpp-targets
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
    write_basic_package_version_file(gmshparsercpp-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    install(
        EXPORT gmshparsercpp-targets
        FILE gmshparsercpp-targets.cmake
        NAMESPACE gmshparsercpp::
        DESTINATION lib/cmake/gmshparsercpp
    )

    install(
        FILES
            "${PROJECT_SOURCE_DIR}/cmake/gmshparsercpp-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/gmshparsercpp-config-version.cmake"
        DESTINATION
            lib/cmake/gmshparsercpp
    )
endif()
