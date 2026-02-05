/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_SETTLE_THE_WORLD_UTIL_H
#define STW_MAP_EDITOR_SETTLE_THE_WORLD_UTIL_H

#include <exdevgfx/framebuffer_8bit.h>

#define MAP_SIZE_X 160
#define MAP_SIZE_Y 128
#define MAP_SIZE_TOTAL 20480
#define TILES_MAP_NUM 96
#define TILES_MAP_COAST_NUM 70
#define TILES_ICON_NUM 2
#define TILES_TOTAL_NUM (TILES_MAP_NUM + TILES_MAP_COAST_NUM + TILES_ICON_NUM)
void stw_read_tiles(Tiles8bit_t *tiles);

int stw_read_map(const char* path, unsigned char map[MAP_SIZE_Y][MAP_SIZE_X]);

int is_ocean_tile(int id);

uint8_t get_shore_tile_id_straight(int id, uint8_t sum);

uint8_t get_shore_tile_id_angular(int id, uint8_t sum);

#endif//STW_MAP_EDITOR_SETTLE_THE_WORLD_UTIL_H
