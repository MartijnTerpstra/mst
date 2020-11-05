cmake_minimum_required (VERSION 3.13)

macro(add_mst_test CATEGORY TEST_NAME)

add_executable("test_${CATEGORY}_${TEST_NAME}" "${CATEGORY}/${TEST_NAME}.cpp")

target_link_libraries("test_${CATEGORY}_${TEST_NAME}"
	PRIVATE mst::mst
	PRIVATE Catch2::Catch2)

target_compile_definitions("test_${CATEGORY}_${TEST_NAME}"
	PRIVATE _MST_TESTING=1)

target_include_directories("test_${CATEGORY}_${TEST_NAME}"
	PUBLIC ${PROJECT_SOURCE_DIR}/Test_All_Platforms/shared
)

if (WIN32)

target_compile_options("test_${CATEGORY}_${TEST_NAME}"
	PRIVATE /arch:AVX2)

elseif(UNIX)

target_compile_options("test_${CATEGORY}_${TEST_NAME}"
	PRIVATE -mavx2)

endif()

ParseAndAddCatchTests("test_${CATEGORY}_${TEST_NAME}")

endmacro(add_mst_test)

add_mst_test(algorithm iterate)
add_mst_test(algorithm iterate_remove)
add_mst_test(algorithm container)

add_mst_test(common common)
add_mst_test(common compiletime)
add_mst_test(common flag)
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

add_mst_test(simd vector)

add_mst_test(threading slim_event)