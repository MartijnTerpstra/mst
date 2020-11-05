cmake_minimum_required (VERSION 3.13)

target_sources(mst PRIVATE
mdll_malloc.cpp
mlogger.cpp
mplatform.cpp
mthreading.cpp
mthreading_rw_lock.cpp
mthreading_slim.cpp)