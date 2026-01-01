/**
* Copyright 2026 Andre Geisler (andre@exdev.de)
 */
#include "ui_status.h"

#include <assert.h>
#include <stdlib.h>

void ui_status_init(UIStatus_t *self, const int x, const int y, const int width, const int height, Tiles8bit_t *tiles) {
    assert(self);

    ui_component_init(&self->base, x, y, width, height);

    self->base.subtype = UI_STATUS_SUBTYPE;
    self->base.functions.destroy_func = (void (*)(void *)) &ui_status_destroy;

    self->properties.current_tile_index = 0;// water
    self->tiles = tiles;

    self->current_tile = ui_icon_create(2, 2, framebuffer_8bit_copy(tiles->tiles));
    self->current_tile->properties.clickable = 0;
    ui_component_connect(self, self->current_tile);
}

UIStatus_t *ui_status_create(const int x, const int y, const int width, const int height, Tiles8bit_t *tiles) {
    UIStatus_t *self = malloc(sizeof(UIStatus_t));

    ui_status_init(self, x, y, width, height, tiles);

    return self;
}

void ui_status_destroy(UIStatus_t *self) {
    assert(self);

    self->tiles = NULL;

    ui_component_destroy(&self->base);
}

void ui_status_update_current_tile_index(UIStatus_t *self, const int current_tile_index) {
    assert(self);

    if (current_tile_index != self->properties.current_tile_index) {
        self->properties.current_tile_index = current_tile_index;
        framebuffer_8bit_copy_to(self->tiles->tiles + current_tile_index, self->current_tile->icon);
        self->current_tile->base.flags.dirty_flag = 1;
        self->base.flags.dirty_flag = 1;
    }
}
