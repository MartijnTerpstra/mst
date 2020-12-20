cmake_minimum_required (VERSION 3.13)

add_mst_test(memory com_ptr)






add_executable(dev dev.cpp)

target_link_libraries(dev
	PRIVATE mst::mst)

target_compile_features(dev
	PUBLIC cxx_std_17)

if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
target_compile_options(dev
	PRIVATE /O2
	PRIVATE /Ob2
	PRIVATE /arch:AVX2
	PRIVATE /fp:fast
	PRIVATE /GS-
	PRIVATE /Oi
	PRIVATE /Ot
	PRIVATE /GL
	PRIVATE /Zo)
endif()