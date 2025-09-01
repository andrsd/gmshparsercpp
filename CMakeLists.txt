cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project(gmshparsercpp
    VERSION 1.4
    LANGUAGES CXX
)

enable_language(C)

include(FetchContent)
include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)

set(LibraryTypeValues "SHARED;STATIC")
if(NOT GMSHPARSERCPP_LIBRARY_TYPE)
    set(GMSHPARSERCPP_LIBRARY_TYPE "SHARED" CACHE STRING "Build as a shared or static library." FORCE)
endif()
set_property(CACHE GMSHPARSERCPP_LIBRARY_TYPE PROPERTY STRINGS ${LibraryTypeValues})

option(GMSHPARSERCPP_BUILD_TESTS "Build tests" NO)

find_package(fmt REQUIRED)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
mark_as_advanced(FORCE
    BUILD_GMOCK
    INSTALL_GTEST
    FETCHCONTENT_SOURCE_DIR_GOOGLETEST
    FETCHCONTENT_UPDATES_DISCONNECTED_GOOGLETEST
)

FetchContent_MakeAvailable(googletest)
mark_as_advanced(FORCE
    FETCHCONTENT_BASE_DIR
    FETCHCONTENT_FULLY_DISCONNECTED
    FETCHCONTENT_QUIET
    FETCHCONTENT_UPDATES_DISCONNECTED
)

add_subdirectory(src)

# Tests

if (GMSHPARSERCPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

add_subdirectory(docs)
