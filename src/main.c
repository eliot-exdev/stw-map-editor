/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include "ui_map.h"
#include "ui_tile.h"

#include <exdevgfx/exdev_base.h>
#include <exdevgfx/logger.h>

#include <stdlib.h>

int main(void) {
    int res = exdev_base_init();
    if (res) {
        log_warning("could not init exdevgfx");
        return res;
    }
    // load tile map
    Framebuffer8Bit_t fb;
    res = framebuffer_8bit_read_from_dat(&fb, "assets/maptiles_8bit.dat");
    if (res) {
        log_warning("could not read assets/maptiles_8bit.dat");
        return res;
    }
    Tiles8bit_t *tiles = malloc(sizeof(Tiles8bit_t));
    tiles_8bit_init_from_framebuffer(tiles, &fb, TILE_WIDTH, TILE_HEIGHT);
    framebuffer_8bit_deinit(&fb);

    // setup application
    UIApplication_t app;
    ui_application_init(&app, UI_WIDTH, UI_HEIGHT);
    res = palette_8bit_read_from_dat(&app.palette, "assets/maptiles_8bit.pal");
    if (res) {
        log_warning("could not read assets/maptiles_8bit.pal");
        return res;
    }

    // map component
    UIMap_t *map_component = ui_map_create(UI_BORDER_SIZE, UI_BORDER_SIZE, UI_MAP_WIDTH, UI_MAP_HEIGHT, tiles);
    ui_component_connect(&app.root, map_component);

    // tile view
    UITile_t *tile_view = ui_tile_create(UI_TILE_X_POS, UI_BORDER_SIZE, UI_TILE_WIDTH, UI_TILE_HEIGHT, tiles, &map_component->properties.current_tile_index);
    ui_component_connect(&app.root, tile_view);

    // run
    ui_application_prepare(&app);
    ui_application_run(&app, UPDATE_INTERVAL);

    // cleanup
    ui_application_destroy(&app);
    exdev_base_deinit();

    return 0;
}
