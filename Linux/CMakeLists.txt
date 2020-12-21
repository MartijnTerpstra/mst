cmake_minimum_required (VERSION 3.13)

target_sources(mst PRIVATE
mplatform.cpp
mthreading.cpp
mthreading_rw_lock.cpp
mthreading_slim.cpp)

## threading lib for mutexes/semaphores
target_link_libraries(mst
	PUBLIC pthread)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
target_compile_options(mst
	PUBLIC 
		-mavx
		$<$<CONFIG:Debug>:-O0 -ggdb3>
		$<$<CONFIG:Release>:-O2>)
endif()