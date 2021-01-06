cmake_minimum_required (VERSION 3.13)

project (mst CXX)

option(MST_RUN_TESTS "Build tests for mst utility library" OFF)

if(UNIX)
set(platform_name "Linux")

elseif(WIN32)
set(platform_name "Windows")

else() ## platform neither linux nor windows

message(FATAL_ERROR "Platform not found")

endif()

add_library (mst STATIC
	All_Platforms/muuid.cpp
	${platform_name}/mplatform.cpp
	${platform_name}/mthreading.cpp
	${platform_name}/mthreading_rw_lock.cpp
	${platform_name}/mthreading_slim.cpp)

add_library(mst::mst ALIAS mst)

target_compile_features(mst 
	PUBLIC cxx_std_11)

target_include_directories(mst
	PUBLIC ${PROJECT_SOURCE_DIR}/mst
)

add_subdirectory(${platform_name})

if (MST_RUN_TESTS)

enable_testing()

include(CTest)

add_subdirectory(dependencies)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)

include (Catch)

macro(add_mst_test CATEGORY TEST_NAME)

add_executable("test_${CATEGORY}_${TEST_NAME}" "tests/${CATEGORY}/${TEST_NAME}.cpp")

target_link_libraries("test_${CATEGORY}_${TEST_NAME}"
	PRIVATE mst::mst
	PRIVATE Catch2::Catch2)

target_compile_definitions("test_${CATEGORY}_${TEST_NAME}"
	PRIVATE _MST_TESTING=1)

target_include_directories("test_${CATEGORY}_${TEST_NAME}"
	PUBLIC ${PROJECT_SOURCE_DIR}/tests/testing
)

catch_discover_tests("test_${CATEGORY}_${TEST_NAME}")

endmacro(add_mst_test)

add_mst_test(algorithm container)
add_mst_test(algorithm for_each)
add_mst_test(algorithm for_each_remove_if)

add_mst_test(common common)
add_mst_test(common compiletime)
add_mst_test(common flag)
add_mst_test(common hash)
add_mst_test(common printf)
add_mst_test(common uuid)

add_mst_test(containers array_view)
add_mst_test(containers arraymap)
add_mst_test(containers colony)
add_mst_test(containers ranges)
add_mst_test(containers sparse_set)

add_mst_test(lock_free queue)

add_mst_test(math algorithm)
add_mst_test(math vector_simd)
add_mst_test(math matrix_simd)
add_mst_test(math matrix_functionality)

add_mst_test(memory aligned_allocator)
add_mst_test(memory aligned_malloc)
add_mst_test(memory memory_view)

add_mst_test(platform directory_and_paths)
add_mst_test(platform memory)
add_mst_test(platform names_and_versions)
add_mst_test(platform processor)

add_mst_test(testing random_data_generator)
add_mst_test(testing set_assertions)

add_mst_test(threading slim_event)

if(WIN32)
add_mst_test(memory com_ptr)
endif()

endif()