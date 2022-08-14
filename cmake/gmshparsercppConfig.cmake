include(CMakeFindDependencyMacro)

find_dependency(fmt REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/gmshparsercppTargets.cmake")
