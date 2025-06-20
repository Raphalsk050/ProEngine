cmake_minimum_required(VERSION 3.13)
project(filament LANGUAGES CXX)

file(GLOB_RECURSE FILAMENT_SRC
        macos/arm64/**/*.a
)

set(FILAMENT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(FILAMENT_LIB ${FILAMENT_SRC})

add_library(filament STATIC IMPORTED GLOBAL)

set_target_properties(filament PROPERTIES
        IMPORTED_LOCATION "${FILAMENT_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${FILAMENT_INCLUDE_DIR}"
        LINKER_LANGUAGE CXX
)
