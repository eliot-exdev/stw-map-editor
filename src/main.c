/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include <exdevgfx/exdev_base.h>
#include <exdevgfx/framebuffer_8bit.h>

int main(void) {
    exdev_base_init();

    // build tile map
    Framebuffer8Bit_t fb;
    framebuffer_8bit_read_from_dat(&fb, "assets/maptiles_8bit.dat");
    Tiles8bit_t tiles;
    tiles_8bit_init_from_framebuffer(&tiles, &fb, 16, 16);
    framebuffer_8bit_deinit(&fb);

    Palette8Bit_t palette;
    palette_8bit_read_from_dat(&palette, "assets/maptiles_8bit.pal");

    // cleanup
    tiles_8bit_deinit(&tiles);
    exdev_base_deinit();

    return 0;
}
