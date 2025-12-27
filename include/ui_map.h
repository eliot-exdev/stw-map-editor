/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_UI_MAP_H
#define STW_MAP_EDITOR_UI_MAP_H

#include "ui_definitions.h"

struct UIMap {
    UIComponent_t base;

    struct {
        int x_pos;
        int y_pos;
        int x_num_tiles;
        int y_num_tiles;
        int current_tile_index;
    } properties;

    struct {
        int dragged;
    } flags;

    Tiles8bit_t *tiles;

    int *map;

    Framebuffer8Bit_t *fb_map;

    int x_last;

    int y_last;
};

typedef struct UIMap UIMap_t;

void ui_map_init(UIMap_t *self, int x, int y, int width, int height, Tiles8bit_t *tiles);

UIMap_t *ui_map_create(int x, int y, int width, int height, Tiles8bit_t *tiles);

void ui_map_destroy(UIMap_t *self);

void ui_map_prepare(UIMap_t *self);

int ui_map_paint(UIMap_t *self, Framebuffer8Bit_t *fb, int x_offset, int y_offset, int width, int height);

void ui_map_update(UIMap_t *self, long time_elapsed, const Event_t *events, int num_events);

#endif //STW_MAP_EDITOR_UI_MAP_H
