/**
* Copyright 2026 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_UI_STATUS_H
#define STW_MAP_EDITOR_UI_STATUS_H
#include <ui_definitions.h>

struct UIStatus {
    UIComponent_t base;
    struct {
        unsigned char tile_index;
    } properties;
    Tiles8bit_t *tiles;
    UIIcon_t *tile; // tile or bonus
    Font_t font;
    UIText_t *current_tile_coordinates;
    UIIcon_t *quit;
    UIIcon_t *save;
    UIIcon_t *variance;
};

typedef struct UIStatus UIStatus_t;

void ui_status_init(UIStatus_t *self, int x, int y, int width, int height, Tiles8bit_t *tiles);

UIStatus_t *ui_status_create(int x, int y, int width, int height, Tiles8bit_t *tiles);

void ui_status_destroy(UIStatus_t *self);

void ui_status_set_tile_index(UIStatus_t *self, unsigned char tile_index);

void ui_status_set_bonus_index(UIStatus_t *self, unsigned char tile_index);

#endif//STW_MAP_EDITOR_UI_STATUS_H
