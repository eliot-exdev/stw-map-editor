/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include <stdlib.h>

#include "ui_map.h"

#include <exdevgfx/exdev_base.h>
#include <exdevgfx/logger.h>

#define WIDTH 640
#define HEIGHT 480
#define UPDATE_INTERVAL 50// ms
#define TILE_WIDTH 16
#define TILE_HEIGHT 16

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
    ui_application_init(&app, WIDTH, HEIGHT);
    res = palette_8bit_read_from_dat(&app.palette, "assets/maptiles_8bit.pal");
    if (res) {
        log_warning("could not read assets/maptiles_8bit.pal");
        return res;
    }

    // map component
    UIMap_t *map_component = ui_map_create(2, 2, 558, 476, tiles);
    ui_component_connect(&app.root, map_component);

    // tile view
    UIScrollContainer_t *tile_view = ui_scroll_container_create(562, 2, 76, 476, UI_SCROLLING_SUPPORT_VERTICAL);
    int i = 0;
    int row = 0;
    while (i < tiles->num) {
        // left
        Framebuffer8Bit_t *left_fb = framebuffer_8bit_copy(tiles->tiles + i);
        UIIcon_t *left = ui_icon_create(2, row * TILE_HEIGHT + row * 2 + 2, left_fb);
        ui_component_connect(tile_view, left);

        // middle
        if (i + 1 < tiles->num) {
            Framebuffer8Bit_t *middle_fb = framebuffer_8bit_copy(tiles->tiles + i + 1);
            UIIcon_t *middle = ui_icon_create(TILE_WIDTH + 6, row * TILE_HEIGHT + row * 2 + 2, middle_fb);
            ui_component_connect(tile_view, middle);
        }

        // right
        if (i + 2 < tiles->num) {
            Framebuffer8Bit_t *right_fb = framebuffer_8bit_copy(tiles->tiles + i + 2);
            UIIcon_t *right = ui_icon_create(2 * TILE_WIDTH + 10, row * TILE_HEIGHT + row * 2 + 2, right_fb);
            ui_component_connect(tile_view, right);
        }
        row += 1;
        i += 3;
    }
    ui_component_connect(&app.root, tile_view);

    // run
    ui_application_prepare(&app);
    ui_application_run(&app, UPDATE_INTERVAL);

    // cleanup
    ui_application_destroy(&app);
    exdev_base_deinit();

    return 0;
}
