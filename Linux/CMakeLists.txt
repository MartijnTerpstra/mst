cmake_minimum_required (VERSION 3.13)

target_sources(mst PRIVATE
mdll_malloc.cpp
mplatform.cpp
mthreading.cpp
mthreading_rw_lock.cpp
mthreading_slim.cpp)

## threading lib for mutexes/semaphores
target_link_libraries(mst
	PUBLIC pthread)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
target_compile_options(mst
	PUBLIC -mavx)
endif()