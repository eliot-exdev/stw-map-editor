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
#define TILES_ICON_NUM 6
#define TILES_BONUS_NUM 15
#define TILES_UNUSED_NUM 7

struct MapTile {
    uint8_t tile_id;
    uint8_t bonus_id;
};
typedef struct MapTile MapTile_t;

#define MapTileArray_t(n) MapTile_t n[MAP_SIZE_Y][MAP_SIZE_X]

void stw_read_tiles(Tiles8bit_t *tiles_map, Tiles8bit_t *tiles_map_coast, Tiles8bit_t *tiles_icon, Tiles8bit_t *tiles_bonus, Framebuffer8Bit_t *colors_mini_map);

int stw_read_map(const char *path, MapTileArray_t(map));

int stw_write_map(const char *path, const MapTileArray_t(map));

int stw_write_new_map(const char *orig_path, const MapTileArray_t(map));

int stw_is_ocean_tile(int id);

uint8_t stw_get_shore_tile_id_straight(int id, uint8_t sum);

uint8_t stw_get_shore_tile_id_angular(int id, uint8_t sum);

void stw_randomize_tile_variants(MapTileArray_t(map));

int stw_has_valid_bonus(const MapTile_t *tile);

#endif//STW_MAP_EDITOR_SETTLE_THE_WORLD_UTIL_H
