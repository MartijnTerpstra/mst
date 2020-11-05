cmake_minimum_required (VERSION 3.13)

if(NOT TARGET Catch2::Catch2)
message(STATUS "Catch2 not found, creating Catch2 target")
add_subdirectory(catch2)
endif()