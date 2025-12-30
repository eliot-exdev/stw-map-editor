/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include "settle_the_world_util.h"
#include "ui_definitions.h"

#define EXDEVGFX2_LOG_LEVEL 2
#include <exdevgfx/logger.h>

#include <stdlib.h>

static int VALID_TILES[] = {
        // row num
        6, 7, 8, 12, 13, 14, 15, 16, 17, 18, 19,                                                 // 0 11
        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 39,                                      // 1 13
        46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,                                  // 2 14
        66, 67, 68, 69, 70, 71, 77, 78, 79,                                                      // 3 9
        86, 87, 88, 89, 90, 91, 97, 99,                                                          // 4 8
        106, 107, 108, 117, 118, 119,                                                            // 5 6
        120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 134, 135, 136, 137, 138, 139,// 6 18
        140, 141, 142, 143, 144, 145, 154, 155, 156, 157, 158, 159,                              // 7 12
        160, 161, 162, 163, 164, 165, 172, 173, 174, 175, 176, 177, 178, 179,                    // 8 14
        180, 181, 182, 183, 184, 185, 192, 193, 194, 195, 196, 197, 198, 199,                    // 9 14
        200, 201, 202, 203, 204, 205, 212, 213, 214, 215, 216, 217, 218, 219,                    // 10 14
        220, 221, 222, 223, 224, 225,                                                            // 11 6
        240, 241, 242, 243, 244, 245,                                                            // 12 6
        260, 261, 262, 263, 264, 265,                                                            // 13 6
        280, 281, 282, 283, 284, 285,                                                            // 14 6
        300, 301, 302, 303, 304, 305                                                             // 15 6
};
static int VALID_TILES_NUM = 163;

Tiles8bit_t *stw_read_tiles() {
    log_info("--> read_tiles()");
    int res = 0;

    // read framebuffer
    Framebuffer8Bit_t fb;
    res = framebuffer_8bit_read_from_dat(&fb, "assets/maptiles_8bit.dat");
    if (res) {
        log_warning("could not read assets/maptiles_8bit.dat");
        log_info("<-- read_tiles()!");
        return NULL;
    }

    // get all tiles
    Tiles8bit_t all_tiles;
    tiles_8bit_init_from_framebuffer(&all_tiles, &fb, TILE_WIDTH, TILE_HEIGHT);
    framebuffer_8bit_deinit(&fb);

    // extract valid tiles
    Tiles8bit_t *tiles = malloc(sizeof(Tiles8bit_t));
    tiles_8bit_init(tiles, VALID_TILES_NUM, TILE_WIDTH, TILE_HEIGHT);
    for (int i = 0; i < VALID_TILES_NUM; i++) {
        framebuffer_8bit_copy_to(all_tiles.tiles + VALID_TILES[i], tiles->tiles + i);
    }

    // cleanup
    tiles_8bit_deinit(&all_tiles);

    log_info("<-- read_tiles()");
    return tiles;
}