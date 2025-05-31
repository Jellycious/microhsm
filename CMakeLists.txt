cmake_minimum_required(VERSION 3.1)
project(microhsm LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

option(CODE_COVERAGE "Enable coverage reporting" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed. Please create a build directory and run cmake from there.")
endif()


if(CODE_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Code coverage enabled")
        add_compile_options(--coverage)
        add_link_options(--coverage)
    else()
        message(WARNING "Code coverage only supported with GCC/Clang")
    endif()
endif()

add_compile_options(
    -Wall
    -Wextra
    -Wconversion
    -Wshadow
    -Wsign-conversion
)

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(BUILD_TESTS)
    add_compile_options(
        -DMICROHSM_CUSTOM_CONFIG # Include custom configuration file
        -I${CMAKE_CURRENT_SOURCE_DIR}/tests/
    )
endif()

add_subdirectory(src/microhsm)

if(BUILD_TESTS)
    add_subdirectory(tests)

    enable_testing()
    add_test(NAME microhsm_tests COMMAND microhsm_tests)
endif()
