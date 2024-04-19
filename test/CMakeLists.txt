project(gmshparsercpp-test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GMSHPARSERCPP_ASSETS_DIR ${PROJECT_SOURCE_DIR}/assets CACHE PATH "" FORCE)
mark_as_advanced(FORCE GMSHPARSERCPP_ASSETS_DIR)

add_executable(${PROJECT_NAME}
    main.cpp
    MshFile_test.cpp
    Quad2D_test.cpp
)
target_code_coverage(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        gmshparsercpp
        gtest_main
        gmock_main
)

configure_file(TestConfig.h.in TestConfig.h)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)
if(GMSHPARSERCPP_CODE_COVERAGE)
    set_tests_properties(
        ${PROJECT_NAME}
        PROPERTIES
            ENVIRONMENT LLVM_PROFILE_FILE=${PROJECT_NAME}.profraw
    )
endif()
