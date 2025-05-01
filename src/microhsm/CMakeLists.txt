add_library(microhsm STATIC)

target_sources(microhsm
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/HSM.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/State.cpp
)

target_include_directories(microhsm
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../../include
)
