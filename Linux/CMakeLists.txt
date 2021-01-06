cmake_minimum_required (VERSION 3.13)

## threading lib for mutexes/semaphores
target_link_libraries(mst
	PUBLIC pthread)