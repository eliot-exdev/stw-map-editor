/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include <exdevgfx/exdev_base.h>
#include <exdevgfx/framebuffer_8bit.h>
#include <exdevgfx/logger.h>
#include <exdevgfx/ui/ui.h>

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
    Tiles8bit_t tiles;
    tiles_8bit_init_from_framebuffer(&tiles, &fb, TILE_WIDTH, TILE_HEIGHT);
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
    UIComponent_t *map_component = ui_component_create(2, 2, 586, 476);
    ui_component_connect(&app.root, map_component);

    // tile view
    UIComponent_t *tile_view = ui_scroll_container_create(590, 2, 48, 476, UI_SCROLLING_SUPPORT_VERTICAL);
    ui_component_connect(&app.root, tile_view);

    // run
    ui_application_prepare(&app);
    ui_application_run(&app, UPDATE_INTERVAL);

    // cleanup
    ui_application_destroy(&app);
    tiles_8bit_deinit(&tiles);
    exdev_base_deinit();

    return 0;
}
