add_executable(microhsm_example_basic
    ${CMAKE_CURRENT_SOURCE_DIR}/basic/example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/basic/Valve.cpp
)

target_link_libraries(microhsm_example_basic PRIVATE microhsm)

add_executable(microhsm_example_macros
    ${CMAKE_CURRENT_SOURCE_DIR}/macros/example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/macros/Valve.cpp
)

target_link_libraries(microhsm_example_macros PRIVATE microhsm)
