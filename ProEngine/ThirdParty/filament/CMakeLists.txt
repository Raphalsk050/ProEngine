cmake_minimum_required(VERSION 3.13)
project(filament LANGUAGES CXX)

# 1) coleta todos os .a numa lista
file(GLOB_RECURSE FILAMENT_LIBS
        "${CMAKE_CURRENT_SOURCE_DIR}/macos/arm64/*.a"
)

# 2) cria um target de INTERFACE (não precisa ter um .a único)
add_library(filament INTERFACE)

# 3) diz onde estão os includes
target_include_directories(filament
        INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# 4) adiciona cada .a à lista de link
target_link_libraries(filament
        INTERFACE
        ${FILAMENT_LIBS}
)
