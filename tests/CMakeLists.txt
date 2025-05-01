add_executable(microhsm_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/unity/unity.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp
)

target_include_directories(microhsm_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/unity
)

target_link_libraries(microhsm_tests PRIVATE
    microhsm
)

enable_testing()
add_test(NAME microhsm_tests COMMAND microhsm_tests)
