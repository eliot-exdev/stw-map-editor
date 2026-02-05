/**
* Copyright 2026 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_UI_STATUS_H
#define STW_MAP_EDITOR_UI_STATUS_H
#include <ui_definitions.h>

struct UIStatus {
    UIComponent_t base;
    struct {
        unsigned char current_tile_index;
    } properties;
    Tiles8bit_t *tiles;
    UIIcon_t *current_tile;
    Font_t font;
    UIText_t *current_tile_coordinates;
    UIIcon_t *save;
};

typedef struct UIStatus UIStatus_t;

void ui_status_init(UIStatus_t *self, int x, int y, int width, int height, Tiles8bit_t *tiles);

UIStatus_t *ui_status_create(int x, int y, int width, int height, Tiles8bit_t *tiles);

void ui_status_destroy(UIStatus_t *self);

void ui_status_update_current_tile_index(UIStatus_t *self, unsigned char current_tile_index);

#endif//STW_MAP_EDITOR_UI_STATUS_H
