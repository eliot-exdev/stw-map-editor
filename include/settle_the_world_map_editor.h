/**
 * Copyright 2026 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_SETTLE_THE_WORLD_MAP_EDITOR_H
#define STW_MAP_EDITOR_SETTLE_THE_WORLD_MAP_EDITOR_H

#include <ui_map.h>
#include <ui_tile.h>
#include <ui_status.h>

struct stw_map_editor {
    UIMap_t *map;
    UITile_t *tile;
    UIStatus_t *status;
};

typedef struct stw_map_editor STWMapEditor_t;

#endif//STW_MAP_EDITOR_SETTLE_THE_WORLD_MAP_EDITOR_H
