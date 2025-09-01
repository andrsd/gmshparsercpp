include(CMakeFindDependencyMacro)

find_dependency(fmt 11 REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/gmshparsercpp-targets.cmake")
