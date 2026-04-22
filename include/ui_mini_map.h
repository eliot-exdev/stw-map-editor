/**
* Copyright 2026 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_UI_MINI_MAP_H
#define STW_MAP_EDITOR_UI_MINI_MAP_H

#include <exdevgfx/ui/ui.h>

struct UIMiniMap {
    UIComponent_t base;
};

typedef struct UIMiniMap UIMiniMap_t;

void ui_mini_map_init(UIMiniMap_t *self, int x, int y, int width, int height);

UIMiniMap_t *ui_mini_map_create(int x, int y, int width, int height);

int ui_mini_map_paint(UIMiniMap_t *self, Framebuffer8Bit_t *fb, int x_offset, int y_offset, int width, int height, void *usr_ptr);

void ui_mini_map_update(UIMiniMap_t *self, long time_elapsed, const Event_t *events, int num_events, struct UIApplication *app, void *usr_ptr);

void ui_mini_map_prepare(UIMiniMap_t *self, void *usr_ptr);

#endif //STW_MAP_EDITOR_UI_MINI_MAP_H
