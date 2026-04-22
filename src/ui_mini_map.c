/**
* Copyright 2026 Andre Geisler (andre@exdev.de)
 */

#include "ui_mini_map.h"
#include "ui_definitions.h"

#define EXDEVGFX2_LOG_LEVEL 2
#include <exdevgfx/logger.h>

#include "settle_the_world_map_editor.h"
#include "settle_the_world_util.h"

#include <stdlib.h>
#include <assert.h>

static void on_close_clicked(struct UIIcon *self, UIApplication_t *app, void *usr_ptr) {
    assert(self);

    if (self->flags.clicked) {
        log_info("close clicked");
        ui_application_stop_modal_dialog(app);
    }
}

void ui_mini_map_init(UIMiniMap_t *self, const int x, const int y, const int width, const int height) {
    assert(self);

    ui_component_init(&self->base, x, y, width, height);

    self->base.subtype = UI_MINI_MAP_SUBTYPE;

    self->base.functions.paint_func = (int (*)(void *, Framebuffer8Bit_t *, int, int, int, int, void *)) ui_mini_map_paint;
    self->base.functions.update_func = (void (*)(void *, long, const Event_t *, int, UIApplication_t *app, void *)) ui_mini_map_update;
    self->base.functions.prepare_func = (void (*)(void *, void *)) ui_mini_map_prepare;
}

UIMiniMap_t *ui_mini_map_create(const int x, const int y, const int width, const int height) {
    UIMiniMap_t *self = malloc(sizeof(UIMiniMap_t));
    ui_mini_map_init(self, x, y, width, height);

    return self;
}

void ui_mini_map_prepare(UIMiniMap_t *self, void *usr_ptr) {
    assert(self);
    assert(usr_ptr);

    ui_component_prepare(&self->base, usr_ptr);

    STWMapEditor_t *editor = usr_ptr;

    UIIcon_t *close_icon = ui_icon_create(self->base.properties.width - TILE_WIDTH - 4, self->base.properties.height - TILE_HEIGHT - 4, framebuffer_8bit_copy(editor->tiles_icon.tiles + 4), NULL);
    close_icon->properties.alpha = PEN_INDEX_CYAN + 8;
    close_icon->functions.on_clicked = on_close_clicked;
    ui_component_connect(self, close_icon);
}

int ui_mini_map_paint(UIMiniMap_t *self, Framebuffer8Bit_t *fb, const int x_offset, const int y_offset, const int width, const int height, void *usr_ptr) {
    assert(self);
    assert(fb);

    if (ui_component_paint(&self->base, fb, x_offset, y_offset, width, height, usr_ptr)) {
        const int x = self->base.properties.x + x_offset;
        const int y = self->base.properties.y + y_offset;
        const STWMapEditor_t *editor = usr_ptr;

        // draw minimap
        for (int yy = 0; yy < MAP_SIZE_Y; ++yy) {
            for (int xx = 0; xx < MAP_SIZE_X; ++xx) {
                const int num = editor->map->map[yy][xx].tile_id / 3;
                const int pixel = editor->colors_mini_map.buffer[num];
                framebuffer_8bit_draw_pixel(fb, x + xx + 2, y + yy + 2, pixel);
            }
        }

        // draw position on screen
        const int x_pos = editor->map->properties.x_pos / TILE_WIDTH + 2;
        const int y_pos = editor->map->properties.y_pos / TILE_HEIGHT + 2;
        const int r_width = (editor->map->base.properties.width - 4) / TILE_WIDTH;
        const int r_height = (editor->map->base.properties.height - 4) / TILE_HEIGHT;
        framebuffer_8bit_draw_rect(fb, x + x_pos, y + y_pos, r_width, r_height, PEN_INDEX_YELLOW);

        return 1;
    }
    return 0;
}

void ui_mini_map_update(UIMiniMap_t *self, const long time_elapsed, const Event_t *events, const int num_events, struct UIApplication *app, void *usr_ptr) {
    assert(self);

    ui_component_update(&self->base, time_elapsed, events, num_events, app, usr_ptr);
}
