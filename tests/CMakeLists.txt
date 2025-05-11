add_executable(microhsm_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/test_runner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unity/unity.c
    ${CMAKE_CURRENT_SOURCE_DIR}/context/TestCTX.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/basic/TestHSM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/basic/basic_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/macros/MacroHSM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/macros/macro_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/history/HistoryHSM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/history/history_tests.cpp
)

target_include_directories(microhsm_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/unity
)

target_link_libraries(microhsm_tests PRIVATE
    microhsm
)

add_definitions(-DUNITY_INCLUDE_CONFIG_H)

enable_testing()
add_test(NAME microhsm_tests COMMAND microhsm_tests)
