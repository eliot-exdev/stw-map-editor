/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include "ui_tile.h"

#define EXDEVGFX2_LOG_LEVEL 2
#include <exdevgfx/logger.h>

#include <assert.h>
#include <stdlib.h>

void ui_tile_init(UITile_t *self, const int x, const int y, const int width, const int height, const Tiles8bit_t *tiles, int *current_tile_index) {
    assert(self);

    ui_scroll_container_init(&self->base, x, y, width, height, UI_SCROLLING_SUPPORT_VERTICAL);

    self->base.base.subtype = UI_TILE_SUBTYPE;
    self->properties.current_tile_index = current_tile_index;

    // create icons for all tiles
    int i = 0;
    int row = 0;
    while (i < tiles->num) {
        // left
        Framebuffer8Bit_t *left_fb = framebuffer_8bit_copy(tiles->tiles + i);
        UIIcon_t *left = ui_icon_create(2, row * (TILE_HEIGHT + 4), left_fb);
        left->functions.on_clicked = (void (*)(UIIcon_t *)) &ui_tile_on_icon_click;
        ui_component_connect(self, left);

        // middle
        if (i + 1 < tiles->num) {
            Framebuffer8Bit_t *middle_fb = framebuffer_8bit_copy(tiles->tiles + i + 1);
            UIIcon_t *middle = ui_icon_create(TILE_WIDTH + 6, row * (TILE_HEIGHT + 4), middle_fb);
            middle->functions.on_clicked = (void (*)(UIIcon_t *)) &ui_tile_on_icon_click;
            ui_component_connect(self, middle);
        }

        // right
        if (i + 2 < tiles->num) {
            Framebuffer8Bit_t *right_fb = framebuffer_8bit_copy(tiles->tiles + i + 2);
            UIIcon_t *right = ui_icon_create(2 * TILE_WIDTH + 10, row * (TILE_HEIGHT + 4), right_fb);
            right->functions.on_clicked = (void (*)(UIIcon_t *)) &ui_tile_on_icon_click;
            ui_component_connect(self, right);
        }
        row += 1;
        i += 3;
    }
}

UITile_t *ui_tile_create(const int x, const int y, const int width, const int height, const Tiles8bit_t *tiles, int *current_tile_index) {
    UITile_t *self = malloc(sizeof(UITile_t));

    ui_tile_init(self, x, y, width, height, tiles, current_tile_index);

    return self;
}

void ui_tile_destroy(UITile_t *self) {
    assert(self);

    ui_scroll_container_destroy(&self->base);
}

void ui_tile_on_icon_click(UIIcon_t *icon) {
    assert(icon);
    if (icon->flags.clicked) {
        ui_tile_on_click((UITile_t *) icon->base.parent);
    }
}

void ui_tile_on_click(UITile_t *self) {
    assert(self);

    for (int i = 0; i < self->base.base.children.size; ++i) {
        const UIIcon_t *icon = (UIIcon_t *) self->base.base.children.components[i];
        if (icon->flags.clicked) {
            *self->properties.current_tile_index = i;
            // log_info_fmt("new tile index: %d", i);
            return;
        }
    }
    log_warning("could not find selected tile -  this should not happen!");
}
