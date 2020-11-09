cmake_minimum_required (VERSION 3.13)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

#include(ParseAndAddCatchTests)

if(NOT TARGET Catch2::Catch2)
message(STATUS "Catch2 not found, creating Catch2 target")
add_subdirectory(catch2)
endif()