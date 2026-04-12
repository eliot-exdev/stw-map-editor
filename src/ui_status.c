/**
* Copyright 2026 Andre Geisler (andre@exdev.de)
 */
#include "ui_status.h"

#include "settle_the_world_map_editor.h"
#include "settle_the_world_util.h"

#define EXDEVGFX2_LOG_LEVEL 2
#include "exdevgfx/logger.h"

#include <assert.h>
#include <stdlib.h>

static void on_quit_clicked(struct UIIcon *self, UIApplication_t *app, void *usr_ptr) {
    assert(self);
    assert(app);

    if (self->flags.clicked) {
        log_info("quit clicked");
        ui_application_quit(app);
    }
}

static void on_variance_clicked(struct UIIcon *self, UIApplication_t *app, void *usr_ptr) {
    assert(self);
    if (self->flags.clicked) {
        log_info("variance started");
        STWMapEditor_t *editor = (STWMapEditor_t *) usr_ptr;
        stw_randomize_tile_variants(editor->map->map);
        ui_map_render_complete_map(editor->map);
        ui_component_set_dirty(&editor->map->base);
        ui_component_set_enable(&editor->status->save->base, 1);
        log_info("variance finished");
    }
}

static void on_save_clicked(struct UIIcon *self, UIApplication_t *app, void *usr_ptr) {
    assert(self);
    assert(usr_ptr);

    if (self->flags.clicked) {
        STWMapEditor_t *editor = (STWMapEditor_t *) usr_ptr;
        int res = 0;
        if (editor->map_path) {
            log_info("writing existing map");
            res = stw_write_map(editor->map_path, editor->map->map);
        } else {
            log_info("writing new map");
            res = stw_write_new_map("maps/new.map", editor->map->map);
        }

        if (res) {
            log_warning("could not write map!");
        } else {
            log_info("successfully wrote map!");
        }
    }
}

void ui_status_init(UIStatus_t *self, const int x, const int y, const int width, const int height, Tiles8bit_t *tiles_map, Tiles8bit_t *tiles_bonus, Tiles8bit_t *tiles_icon) {
    assert(self);

    ui_component_init(&self->base, x, y, width, height);

    self->base.subtype = UI_STATUS_SUBTYPE;
    self->base.functions.destroy_func = (void (*)(void *)) &ui_status_destroy;

    self->properties.tile_index = 0;// water
    self->tiles_map = tiles_map;
    self->tiles_bonus = tiles_bonus;

    self->tile = ui_icon_create(2, 2, framebuffer_8bit_copy(tiles_map->tiles), NULL);
    self->tile->properties.clickable = 0;
    self->tile->properties.alpha = PEN_INDEX_CYAN + 8;
    ui_component_connect(self, self->tile);

    font_init(&self->font, FONT_TYPE_TOPAZ_8);

    self->current_tile_coordinates = ui_text_create(2 + self->tile->base.properties.width + 2, 2, 150, self->tile->base.properties.height, "X: 000 Y: 000", &self->font);
    self->current_tile_coordinates->base.flags.draw_border = 0;
    ui_component_connect(self, self->current_tile_coordinates);

    self->quit = ui_icon_create(width - 20, 2, framebuffer_8bit_copy(tiles_icon->tiles + 2), NULL);
    self->quit->properties.alpha = PEN_INDEX_CYAN + 8;
    self->quit->functions.on_clicked = on_quit_clicked;
    ui_component_connect(self, self->quit);

    self->save = ui_icon_create(width - 39, 2, framebuffer_8bit_copy(tiles_icon->tiles), framebuffer_8bit_copy(tiles_icon->tiles + 1));
    self->save->base.flags.enabled_flag = 0;
    self->save->properties.alpha = PEN_INDEX_CYAN + 8;
    self->save->functions.on_clicked = on_save_clicked;
    ui_component_connect(self, self->save);

    self->variance = ui_icon_create(width - 58, 2, framebuffer_8bit_copy(tiles_icon->tiles + 3), NULL);
    self->variance->properties.alpha = PEN_INDEX_CYAN + 8;
    self->variance->functions.on_clicked = on_variance_clicked;
    ui_component_connect(self, self->variance);
}

UIStatus_t *ui_status_create(const int x, const int y, const int width, const int height, Tiles8bit_t *tiles_map, Tiles8bit_t *tiles_bonus, Tiles8bit_t *tiles_icon) {
    UIStatus_t *self = malloc(sizeof(UIStatus_t));

    ui_status_init(self, x, y, width, height, tiles_map, tiles_bonus, tiles_icon);

    return self;
}

void ui_status_destroy(UIStatus_t *self) {
    assert(self);

    ui_component_destroy(&self->base);

    self->tiles_map = NULL;
    self->tiles_bonus = NULL;
    font_deinit(&self->font);
}

void ui_status_set_tile_index(UIStatus_t *self, const unsigned char tile_index) {
    assert(self);

    if (tile_index != self->properties.tile_index) {
        self->properties.tile_index = tile_index;
        framebuffer_8bit_copy_to(self->tiles_map->tiles + tile_index, self->tile->icon_enabled);
        ui_component_set_dirty(&self->base);
    }
}

void ui_status_set_bonus_index(UIStatus_t *self, unsigned char tile_index) {
    assert(self);

    if (tile_index != self->properties.tile_index) {
        self->properties.tile_index = tile_index;
        framebuffer_8bit_copy_to(self->tiles_bonus->tiles + tile_index, self->tile->icon_enabled);
        ui_component_set_dirty(&self->base);
    }
}