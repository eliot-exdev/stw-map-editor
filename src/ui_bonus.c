/**
* Copyright 2026 Andre Geisler (andre@exdev.de)
*/

#include "ui_bonus.h"

#include "settle_the_world_map_editor.h"

#define EXDEVGFX2_LOG_LEVEL 2
#include <exdevgfx/logger.h>

#include <assert.h>
#include <stdlib.h>

void ui_bonus_init(UIBonus_t *self, const int x, const int y, const int width, const int height, const Tiles8bit_t *tiles) {
    assert(self);

    ui_component_init(&self->base, x, y, width, height);

    self->base.subtype = UI_TILE_SUBTYPE;
    self->base.functions.destroy_func = (void (*)(void *)) &ui_bonus_destroy;

    // create icons for all tiles
    int i = TILES_MAP_NUM + TILES_MAP_COAST_NUM + TILES_ICON_NUM;
    int row = 0;
    while (i < TILES_TOTAL_NUM) {
        // left
        {
            Framebuffer8Bit_t *left_fb = framebuffer_8bit_copy(tiles->tiles + i);
            UIIcon_t *left = ui_icon_create(0, row * (TILE_HEIGHT + 4), left_fb, NULL);
            left->properties.alpha = PEN_INDEX_CYAN + 8;
            left->functions.on_clicked = &ui_bonus_on_icon_click;
            ui_component_connect(self, left);
        }

        // middle
        if (i + 1 < tiles->num) {
            Framebuffer8Bit_t *middle_fb = framebuffer_8bit_copy(tiles->tiles + i + 1);
            UIIcon_t *middle = ui_icon_create(20, row * (TILE_HEIGHT + 4), middle_fb, NULL);
            middle->properties.alpha = PEN_INDEX_CYAN + 8;
            middle->functions.on_clicked = &ui_bonus_on_icon_click;
            ui_component_connect(self, middle);
        }

        // right
        if (i + 2 < tiles->num) {
            Framebuffer8Bit_t *right_fb = framebuffer_8bit_copy(tiles->tiles + i + 2);
            UIIcon_t *right = ui_icon_create(40, row * (TILE_HEIGHT + 4), right_fb, NULL);
            right->properties.alpha = PEN_INDEX_CYAN + 8;
            right->functions.on_clicked = &ui_bonus_on_icon_click;
            ui_component_connect(self, right);
        }
        row += 1;
        i += 3;
    }
}

UIBonus_t *ui_bonus_create(const int x, const int y, const int width, const int height, const Tiles8bit_t *tiles) {
    UIBonus_t *self = malloc(sizeof(UIBonus_t));

    ui_bonus_init(self, x, y, width, height, tiles);

    return self;
}

void ui_bonus_destroy(UIBonus_t *self) {
    assert(self);

    ui_component_destroy(&self->base);
}

void ui_bonus_on_icon_click(UIIcon_t *icon, UIApplication_t *app, void *usr_ptr) {
    assert(icon);
    if (icon->flags.clicked) {
        ui_bonus_on_click((UIBonus_t *) icon->base.parent, app, usr_ptr);
    }
}

void ui_bonus_on_click(UIBonus_t *self, UIApplication_t *app, void *usr_ptr) {
    assert(self);

    for (int i = 0; i < self->base.children.size; ++i) {
        const UIIcon_t *icon = (UIIcon_t *) self->base.children.components[i];
        if (icon->flags.clicked) {
            STWMapEditor_t *editor = (STWMapEditor_t *) usr_ptr;
            ui_map_set_bonus_index(editor->map, i);
            ui_status_set_bonus_index(editor->status, i);
            return;
        }
    }
    log_warning("could not find selected bonus - this should not happen!");
}
