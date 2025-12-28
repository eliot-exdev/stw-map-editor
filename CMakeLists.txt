cmake_minimum_required(VERSION 4.0)

project(stw_map_editor C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(exdev-gfx)

add_executable(stw_map_editor
        src/main.c
        src/ui_map.c
        src/ui_tile.c
)
if (LOW_RESOLUTION)
    target_compile_definitions(stw_map_editor PRIVATE LOW_RESOLUTION)
    message(STATUS "enabled lores")
endif ()

target_include_directories(stw_map_editor
        PRIVATE include
)
target_link_libraries(stw_map_editor
        PRIVATE exdevgfx
)
