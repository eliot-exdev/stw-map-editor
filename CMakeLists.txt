cmake_minimum_required(VERSION 4.0)

project(stw_map_editor C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(exdev-gfx)

add_executable(stw_map_editor
        src/main.c
)
target_include_directories(stw_map_editor
    PRIVATE include
)
