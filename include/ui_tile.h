/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_UI_TILE_H
#define STW_MAP_EDITOR_UI_TILE_H

#include "ui_definitions.h"

struct UITile {
    UIScrollContainer_t base;
    struct {
        int *current_tile_index;
    } properties;
};

typedef struct UITile UITile_t;

void ui_tile_init(UITile_t *self, int x, int y, int width, int height, const Tiles8bit_t *tiles, int *current_tile_index);

UITile_t *ui_tile_create(int x, int y, int width, int height, const Tiles8bit_t *tiles, int *current_tile_index);

void ui_tile_destroy(UITile_t *self);

void ui_tile_on_icon_click(UIIcon_t *icon);

void ui_tile_on_click(UITile_t *self);

#endif//STW_MAP_EDITOR_UI_TILE_H
