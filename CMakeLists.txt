cmake_minimum_required (VERSION 3.13)

project (mst CXX)

option(MST_RUN_TESTS "Build tests for mst utility library" OFF)

add_library(mst STATIC
    src/mplatform.cpp
    src/mthreading.cpp
    src/mthreading_rw_lock.cpp
    src/muuid.cpp)

target_compile_features(mst 
    PUBLIC cxx_std_17)

if(APPLE OR UNIX)
target_link_libraries(mst
    PUBLIC pthread)
endif()

add_library(mst::mst ALIAS mst)

target_include_directories(mst PUBLIC "${PROJECT_SOURCE_DIR}/mst")

if(MST_RUN_TESTS)

option(MST_TEST_ARGS "Arguments for the Catch2 test runner" "")

enable_testing()

include(CTest)


Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.13.1
    )

FetchContent_MakeAvailable(Catch2)

list (APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/contrib")

include(Catch)

function(SETUP_TARGET TARGET_NAME)
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)
    target_link_libraries(${TARGET_NAME} PRIVATE mst)

    target_include_directories(${TARGET_NAME}
        PUBLIC "${PROJECT_SOURCE_DIR}/tests/test_util"
    )

    if(MSVC)
        target_compile_options(
            ${TARGET_NAME}
            PRIVATE
                /EHsc /W1 /wd4996 /w14800
                $<$<CONFIG:Debug>:/Od>
                $<$<CONFIG:Release>:/O2>
        )
    else()
        target_compile_options(
            ${TARGET_NAME}
            PRIVATE
                -pedantic -fvisibility=hidden -Wall -Wshadow -Wno-deprecated-declarations -mavx
                $<$<CONFIG:Debug>:-O0 -ggdb3>
                $<$<CONFIG:Release>:-O2>
        )
    endif()

    target_compile_definitions(${TARGET_NAME} PRIVATE ${ARGN})
endfunction()

if (NOT MST_TEST_ARGS)
set(MST_TEST_ARGS "")
endif()

function(SETUP_BASIC_TEST TEST_NAME TEST_SOURCES)

    add_executable(${TEST_NAME} ${TEST_SOURCES})
    target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2)
    SETUP_TARGET(${TEST_NAME} ${ARGN})
    catch_discover_tests(${TEST_NAME}
        TEST_SPEC ${MST_TEST_ARGS})

endfunction()

macro(add_mst_test CATEGORY TEST_NAME)


SETUP_BASIC_TEST("test_${CATEGORY}_${TEST_NAME}" "tests/${CATEGORY}/${TEST_NAME}.cpp")

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

add_mst_test(test_util random_data_generator)
add_mst_test(test_util set_assertions)

add_mst_test(threading slim_event)

if(WIN32)
add_mst_test(memory com_ptr)
endif()

endif()