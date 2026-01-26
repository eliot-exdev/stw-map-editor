/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_SETTLE_THE_WORLD_UTIL_H
#define STW_MAP_EDITOR_SETTLE_THE_WORLD_UTIL_H

#include <exdevgfx/framebuffer_8bit.h>

#define MAP_SIZE_X 160
#define MAP_SIZE_Y 128
#define MAP_SIZE_TOTAL 20480

void stw_read_tiles(Tiles8bit_t *tiles);

int stw_read_map(const char* path, unsigned char map[MAP_SIZE_Y][MAP_SIZE_X]);

#endif//STW_MAP_EDITOR_SETTLE_THE_WORLD_UTIL_H
