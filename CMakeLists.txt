cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project(gmshparsercpp
    VERSION 2.0.0
    LANGUAGES CXX
)

include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)

set(LibraryTypeValues "SHARED;STATIC")
if(NOT GMSHPARSERCPP_LIBRARY_TYPE)
    set(GMSHPARSERCPP_LIBRARY_TYPE "SHARED" CACHE STRING "Build as a shared or static library." FORCE)
endif()
set_property(CACHE GMSHPARSERCPP_LIBRARY_TYPE PROPERTY STRINGS ${LibraryTypeValues})

option(GMSHPARSERCPP_BUILD_TESTS "Build tests" NO)
option(GMSHPARSERCPP_INSTALL "Install the library" ON)
mark_as_advanced(FORCE GMSHPARSERCPP_INSTALL)

find_package(fmt 11 REQUIRED)

add_subdirectory(src)

# Tests

if (GMSHPARSERCPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

add_subdirectory(docs)
