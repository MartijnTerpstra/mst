cmake_minimum_required (VERSION 3.13)

project (mst CXX)

option(MST_RUN_TESTS "Build tests for mst utility library" False)

if (UNIX)
set(platform_name "Linux")

elseif(WIN32)
set(platform_name "Windows")

else () ## platform neither linux nor windows

message(FATAL_ERROR "Platform not found")

endif ()

# add all platforms directory
add_subdirectory(All_Platforms)

# add source directory
add_subdirectory(${platform_name})

if (MST_RUN_TESTS)

enable_testing()

include(CTest)

add_subdirectory(dependencies)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)

include (Catch)

add_subdirectory("Test_All_Platforms")
add_subdirectory("Test_${platform_name}")

endif()