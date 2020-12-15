cmake_minimum_required (VERSION 3.13)

set (SOURCES
muuid.cpp)

add_library (mst STATIC ${SOURCES})

add_library(mst::mst ALIAS mst)

target_include_directories(mst
	PUBLIC ${PROJECT_SOURCE_DIR}/mst
)

target_compile_features(mst 
	PUBLIC cxx_std_11)

if(MST_COVERAGE)

target_compile_options(mst
	PUBLIC -O0
	PUBLIC --coverage 
	PUBLIC -fno-inline
	PUBLIC -fno-inline-small-functions
	PUBLIC -fno-default-inline)

endif()