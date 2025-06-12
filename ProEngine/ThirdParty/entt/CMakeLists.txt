project(Entt LANGUAGES CXX)

add_library(Entt INTERFACE)

target_include_directories(Entt
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)