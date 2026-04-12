/**
 * Copyright 2026 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_SETTLE_THE_WORLD_MAP_EDITOR_H
#define STW_MAP_EDITOR_SETTLE_THE_WORLD_MAP_EDITOR_H

#include <ui_map.h>
#include <ui_tile.h>
#include <ui_status.h>
#include <ui_bonus.h>

struct stw_map_editor {
    UIMap_t *map;
    UITile_t *tile;
    UIBonus_t *bonus;
    UIStatus_t *status;
    const char *map_path;
    Tiles8bit_t tiles_map;
    Tiles8bit_t tiles_map_coast;
    Tiles8bit_t tiles_icon;
    Tiles8bit_t tiles_bonus;
    Framebuffer8Bit_t colors_mini_map;
};

typedef struct stw_map_editor STWMapEditor_t;

#endif//STW_MAP_EDITOR_SETTLE_THE_WORLD_MAP_EDITOR_H
