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
        log_warning("variance clicked - not implemented!");
    }
}

static void on_save_clicked(struct UIIcon *self, UIApplication_t *app, void *usr_ptr) {
    assert(self);
    assert(usr_ptr);

    if (self->flags.clicked) {
        STWMapEditor_t *editor = (STWMapEditor_t *) usr_ptr;
        const int res = stw_write_map(editor->map_path, editor->map->map);
        if (res) {
            log_warning("could not write map!");
        } else {
            log_info("successfully wrote map!");
        }
    }
}

void ui_status_init(UIStatus_t *self, const int x, const int y, const int width, const int height, Tiles8bit_t *tiles) {
    assert(self);

    ui_component_init(&self->base, x, y, width, height);

    self->base.subtype = UI_STATUS_SUBTYPE;
    self->base.functions.destroy_func = (void (*)(void *)) &ui_status_destroy;

    self->properties.current_tile_index = 0;// water
    self->tiles = tiles;

    self->current_tile = ui_icon_create(2, 2, framebuffer_8bit_copy(tiles->tiles), NULL);
    self->current_tile->properties.clickable = 0;
    ui_component_connect(self, self->current_tile);

    font_init(&self->font, FONT_TYPE_TOPAZ_8);

    self->current_tile_coordinates = ui_text_create(2 + self->current_tile->base.properties.width + 2, 2, 150, self->current_tile->base.properties.height, "X: 000 Y: 000", &self->font);
    self->current_tile_coordinates->base.flags.draw_border = 0;
    ui_component_connect(self, self->current_tile_coordinates);

    self->quit = ui_icon_create(width - 20, 2, framebuffer_8bit_copy(tiles->tiles + TILES_TOTAL_NUM - 2), NULL);
    self->quit->properties.alpha = PEN_INDEX_CYAN + 8;
    self->quit->functions.on_clicked = on_quit_clicked;
    ui_component_connect(self, self->quit);

    self->save = ui_icon_create(width - 39, 2, framebuffer_8bit_copy(tiles->tiles + TILES_TOTAL_NUM - 4), framebuffer_8bit_copy(tiles->tiles + TILES_TOTAL_NUM - 3));
    self->save->base.flags.enabled_flag = 0;
    self->save->properties.alpha = PEN_INDEX_CYAN + 8;
    self->save->functions.on_clicked = on_save_clicked;
    ui_component_connect(self, self->save);

    self->variance = ui_icon_create(width - 58, 2, framebuffer_8bit_copy(tiles->tiles + TILES_TOTAL_NUM - 1), NULL);
    self->variance->properties.alpha = PEN_INDEX_CYAN + 8;
    self->variance->functions.on_clicked = on_variance_clicked;
    ui_component_connect(self, self->variance);
}

UIStatus_t *ui_status_create(const int x, const int y, const int width, const int height, Tiles8bit_t *tiles) {
    UIStatus_t *self = malloc(sizeof(UIStatus_t));

    ui_status_init(self, x, y, width, height, tiles);

    return self;
}

void ui_status_destroy(UIStatus_t *self) {
    assert(self);

    ui_component_destroy(&self->base);

    self->tiles = NULL;
    font_deinit(&self->font);
}

void ui_status_update_current_tile_index(UIStatus_t *self, const unsigned char current_tile_index) {
    assert(self);

    if (current_tile_index != self->properties.current_tile_index) {
        self->properties.current_tile_index = current_tile_index;
        framebuffer_8bit_copy_to(self->tiles->tiles + current_tile_index, self->current_tile->icon_enabled);
        ui_component_set_dirty(&self->base);
    }
}
