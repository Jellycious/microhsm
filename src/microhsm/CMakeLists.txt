add_library(microhsm STATIC)

target_sources(microhsm
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/objects/BaseHSM.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/objects/BaseState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/objects/Vertex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/objects/History.cpp
)

target_include_directories(microhsm
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)
