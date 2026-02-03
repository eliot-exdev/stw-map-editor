/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include "ui_map.h"

#define EXDEVGFX2_LOG_LEVEL 2
#include "settle_the_world_map_editor.h"


#include <exdevgfx/logger.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void ui_map_init(UIMap_t *self, const int x, const int y, const int width, const int height, Tiles8bit_t *tiles) {
    assert(self);
    assert(tiles);

    ui_component_init(&self->base, x, y, width, height);

    self->base.subtype = UI_MAP_SUBTYPE;

    self->base.functions.destroy_func = (void (*)(void *)) &ui_map_destroy;
    self->base.functions.paint_func = (int (*)(void *, Framebuffer8Bit_t *, int, int, int, int, void *)) ui_map_paint;
    self->base.functions.update_func = (void (*)(void *, long, const Event_t *, int, void *)) ui_map_update;
    self->base.functions.prepare_func = (void (*)(void *, void *)) ui_map_prepare;

    self->properties.x_pos = 0;
    self->properties.y_pos = 0;
    self->properties.current_tile_index = 0;// water
    self->flags.dragged = 0;

    self->tiles = tiles;

    self->fb_map = malloc(sizeof(Framebuffer8Bit_t));
    framebuffer_8bit_init(self->fb_map, MAP_SIZE_X * TILE_WIDTH, MAP_SIZE_Y * TILE_HEIGHT);
    framebuffer_8bit_fill(self->fb_map, self->base.properties.background_color);

    self->x_last = 0;
    self->y_last = 0;
}

UIMap_t *ui_map_create(const int x, const int y, const int width, const int height, Tiles8bit_t *tiles) {
    UIMap_t *self = malloc(sizeof(UIMap_t));
    ui_map_init(self, x, y, width, height, tiles);
    return self;
}

void ui_map_destroy(UIMap_t *self) {
    assert(self);

    self->tiles = NULL;

    framebuffer_8bit_deinit(self->fb_map);
    free(self->fb_map);
    self->fb_map = NULL;

    ui_component_destroy(&self->base);
}

#define SHORE_BORDER 2
static void draw_tile(UIMap_t *self, const int x, const int y, const uint8_t id) {
    const Framebuffer8Bit_t *tile = &self->tiles->tiles[id];
    if (!is_ocean_tile(id)) {
        framebuffer_8bit_draw_framebuffer(self->fb_map, x * TILE_WIDTH, y * TILE_HEIGHT, tile);
    } else if (x < SHORE_BORDER || y < SHORE_BORDER || x >= MAP_SIZE_X - SHORE_BORDER || y >= MAP_SIZE_Y - SHORE_BORDER) {
        framebuffer_8bit_draw_framebuffer(self->fb_map, x * TILE_WIDTH, y * TILE_HEIGHT, tile);
    } else {

        // look straight
        uint8_t sum = is_ocean_tile(self->map[y - 1][x]) ? 0 : 1;
        sum += is_ocean_tile(self->map[y][x + 1]) ? 0 : 2;
        sum += is_ocean_tile(self->map[y + 1][x]) ? 0 : 4;
        sum += is_ocean_tile(self->map[y][x - 1]) ? 0 : 8;

        uint8_t new_id = get_shore_tile_id_straight(id, sum);
        if (new_id != id) {
            tile = &self->tiles->tiles[get_shore_tile_id_straight(new_id, sum)];
        } else {
            // look angular
            sum = 0;
            sum += is_ocean_tile(self->map[y - 1][x + 1]) ? 0 : 16;
            sum += is_ocean_tile(self->map[y + 1][x + 1]) ? 0 : 32;
            sum += is_ocean_tile(self->map[y + 1][x - 1]) ? 0 : 64;
            sum += is_ocean_tile(self->map[y - 1][x - 1]) ? 0 : 128;
            new_id = get_shore_tile_id_angular(id, sum);
            if (new_id != id) {
                tile = &self->tiles->tiles[get_shore_tile_id_straight(new_id, sum)];
            }
        }
        framebuffer_8bit_draw_framebuffer(self->fb_map, x * TILE_WIDTH, y * TILE_HEIGHT, tile);
    }
}

void ui_map_prepare(UIMap_t *self, void *usr_ptr) {
    assert(self);

    ui_component_prepare(&self->base, usr_ptr);

    // init tiles with water
    for (int y = 0; y < MAP_SIZE_Y; y++) {
        for (int x = 0; x < MAP_SIZE_X; x++) {
            self->map[y][x] = self->properties.current_tile_index;
        }
    }

    // read map from file
    const STWMapEditor_t *editor = usr_ptr;
    if (editor->map_path) {
        if (stw_read_map(editor->map_path, self->map)) {
            log_warning_fmt("failed to read map from: %s", editor->map_path);
        }
    }
    // draw tiles
    for (int y = 0; y < MAP_SIZE_Y; y++) {
        for (int x = 0; x < MAP_SIZE_X; x++) {
            draw_tile(self, x, y, self->map[y][x]);
        }
    }
}

int ui_map_paint(UIMap_t *self, Framebuffer8Bit_t *fb, const int x_offset, const int y_offset, const int width, const int height, void *usr_ptr) {
    assert(self);
    assert(fb);
    int res = ui_component_paint(&self->base, fb, x_offset, y_offset, width, height, usr_ptr);

    if (res || self->base.flags.dirty_flag) {
        const int x = self->base.properties.x + x_offset;
        const int y = self->base.properties.y + y_offset;
        framebuffer_8bit_blit_8bit(fb,
                                   self->fb_map,
                                   self->properties.x_pos,
                                   self->properties.y_pos,
                                   self->base.properties.width - 4,
                                   self->base.properties.height - 4,
                                   x + 2,
                                   y + 2);
        res = 1;
        self->base.flags.dirty_flag = 0;
    }

    return res;
}

void ui_map_update(UIMap_t *self, const long time_elapsed, const Event_t *events, const int num_events, void *usr_ptr) {
    assert(self);

    for (int i = 0; i < num_events; ++i) {
        if (events[i].type == EVENT_MOUSE) {
            // mouse button right
            if (events[i].mouse_event.button == MOUSE_BUTTON_1) {
                if (events[i].mouse_event.event == MOUSE_EVENT_BUTTON_PRESSED) {
                    if (ui_component_is_inside(&self->base, events[i].mouse_event.position_x, events[i].mouse_event.position_y)) {
                        log_debug("ui_map_update dragged");
                        int x = events[i].mouse_event.position_x - 2;
                        int y = events[i].mouse_event.position_y - 2;
                        ui_component_get_relative_position(&self->base, &x, &y);
                        self->flags.dragged = 1;
                        self->x_last = x;
                        self->y_last = y;
                    }
                } else if (self->flags.dragged && events[i].mouse_event.event == MOUSE_EVENT_BUTTON_RELEASED) {
                    log_debug("ui_map_update released");
                    self->flags.dragged = 0;
                }
            }
            // mouse button left
            else if (events[i].mouse_event.button == MOUSE_BUTTON_0 && events[i].mouse_event.event == MOUSE_EVENT_BUTTON_PRESSED) {
                if (ui_component_is_inside(&self->base, events[i].mouse_event.position_x, events[i].mouse_event.position_y)) {
                    int x = events[i].mouse_event.position_x - 2;
                    int y = events[i].mouse_event.position_y - 2;
                    ui_component_get_relative_position(&self->base, &x, &y);
                    x = (self->properties.x_pos + x) / TILE_WIDTH;
                    y = (self->properties.y_pos + y) / TILE_HEIGHT;

                    // update tile
                    self->map[y][x] = self->properties.current_tile_index;

                    // render
                    const Framebuffer8Bit_t *tile = &self->tiles->tiles[self->map[y][x]];
                    framebuffer_8bit_draw_framebuffer(self->fb_map, x * TILE_WIDTH, y * TILE_HEIGHT, tile);
                    self->base.flags.dirty_flag = 1;
                }
            }
            // mouse moved
            else if (events[i].mouse_event.event == MOUSE_EVENT_MOVED && (ui_component_is_inside(&self->base, events[i].mouse_event.position_x, events[i].mouse_event.position_y))) {
                int x = events[i].mouse_event.position_x - 2;
                int y = events[i].mouse_event.position_y - 2;
                ui_component_get_relative_position(&self->base, &x, &y);

                // move map
                if (self->flags.dragged) {
                    if (x > self->x_last) {
                        self->properties.x_pos -= x - self->x_last;
                    } else {
                        self->properties.x_pos += self->x_last - x;
                    }

                    if (y > self->y_last) {
                        self->properties.y_pos -= y - self->y_last;
                    } else {
                        self->properties.y_pos += self->y_last - y;
                    }

                    self->x_last = x;
                    self->y_last = y;

                    if (self->properties.x_pos < 0) {
                        self->properties.x_pos = 0;
                    } else if (self->properties.x_pos > self->fb_map->width - self->base.properties.width + 4) {
                        self->properties.x_pos = self->fb_map->width - self->base.properties.width + 4;
                    }

                    if (self->properties.y_pos < 0) {
                        self->properties.y_pos = 0;
                    } else if (self->properties.y_pos > self->fb_map->height - self->base.properties.height + 4) {
                        self->properties.y_pos = self->fb_map->height - self->base.properties.height + 4;
                    }
                    self->base.flags.dirty_flag = 1;
                    log_debug_fmt("x=%d, y=%d", self->properties.x_pos, self->properties.y_pos);
                } else {
                    STWMapEditor_t *editor = (STWMapEditor_t *) usr_ptr;
                    const int tile_x = (self->properties.x_pos + x) / TILE_WIDTH;
                    const int tile_y = (self->properties.y_pos + y) / TILE_HEIGHT;
                    char *text = malloc(20);
                    memset(text, 0, 20);
                    sprintf(text, "X: %03d Y: %03d", tile_x, tile_y);
                    ui_text_update_text(editor->status->current_tile_coordinates, text);
                    free(text);
                }
            }
        }
    }
}

void ui_map_update_current_tile_index(UIMap_t *self, const unsigned char current_tile_index) {
    assert(self);

    self->properties.current_tile_index = current_tile_index;
}
