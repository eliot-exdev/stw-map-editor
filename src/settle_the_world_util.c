/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include "settle_the_world_util.h"
#include "ui_definitions.h"

#define EXDEVGFX2_LOG_LEVEL 2
#include <exdevgfx/logger.h>

#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

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

void stw_read_tiles(Tiles8bit_t *tiles) {
    int res = 0;

    // read framebuffer
    Framebuffer8Bit_t fb;
    res = framebuffer_8bit_read_from_dat(&fb, "assets/maptiles_8bit.dat");
    if (res) {
        log_warning("could not read assets/maptiles_8bit.dat");
        return;
    }

    // get all tiles
    Tiles8bit_t all_tiles;
    tiles_8bit_init_from_framebuffer(&all_tiles, &fb, TILE_WIDTH, TILE_HEIGHT);
    framebuffer_8bit_deinit(&fb);

    // extract valid tiles
    tiles_8bit_init(tiles, VALID_TILES_NUM, TILE_WIDTH, TILE_HEIGHT);
    for (int i = 0; i < VALID_TILES_NUM; i++) {
        framebuffer_8bit_copy_to(all_tiles.tiles + VALID_TILES[i], tiles->tiles + i);
    }

    // cleanup
    tiles_8bit_deinit(&all_tiles);
}

enum MAP_TILE_IDS {
    MAP_TILE_ID_OCEAN = 0,
    MAP_TILE_ID_GRASS = 1,
    MAP_TILE_ID_BEACH = 2,
    MAP_TILE_ID_PRAIRIE = 3,
    MAP_TILE_ID_DESERT = 4,
    MAP_TILE_ID_DRYLAND = 5,
    MAP_TILE_ID_WETLAND = 6,
    MAP_TILE_ID_SWAMP = 7,
    MAP_TILE_ID_SOIL = 8,
    MAP_TILE_ID_COLDLAND = 9,
    MAP_TILE_ID_TUNDRA = 10,
    MAP_TILE_ID_ICELAND = 11,
    MAP_TILE_ID_PLAINS = 12,
    MAP_TILE_ID_SAVANNAH = 13,
    MAP_TILE_ID_SALTPONDS = 14,
    MAP_TILE_ID_CLAYPIT = 15,
    MAP_TILE_ID_LMOUNTAIN = 16,
    MAP_TILE_ID_IRONHILLS = 17,
    MAP_TILE_ID_MMOUNTAIN = 18,
    MAP_TILE_ID_MARBLEMOUNTAIN = 19,
    MAP_TILE_ID_HMOUNTAIN = 20,
    MAP_TILE_ID_TMOUNTAIN = 21,
    MAP_TILE_ID_LAKE = 22,
    MAP_TILE_ID_GRASSFOREST = 23,
    MAP_TILE_ID_PRAIRIEFOREST = 24,
    MAP_TILE_ID_DESERTFOREST = 25,
    MAP_TILE_ID_DRYLANDFOREST = 26,
    MAP_TILE_ID_WETLANDFOREST = 27,
    MAP_TILE_ID_SWAMPFOREST = 28,
    MAP_TILE_ID_COLDLANDFOREST = 29,
    MAP_TILE_ID_LMOUNTAINFOREST = 32,
    MAP_TILE_ID_MMOUNTAINFOREST = 33,
    MAP_TILE_ID_TROPICALFOREST = 34
};

struct MapInfo {
    uint8_t id;
    uint8_t object_id;
    uint16_t style_id;
    uint16_t unit;
    uint16_t city;
    uint8_t variance;
    uint8_t village;
    uint8_t road;
    uint8_t visible;
    uint8_t unit_visible;
    uint8_t dirty;
    uint8_t continent;
    uint8_t obstacle;
};
typedef struct MapInfo MapInfo_t;

static int check_id(const uint8_t id) {
    return id < 35;
}
static int check_variance(const uint8_t variance) {
    return variance > 0 && variance < 4;
}

int stw_read_map(const char *path) {
    assert(path);

    FILE *fp = fopen(path, "r");
    if (!fp) {
        log_warning_fmt("could not read map from: %s", path);
        return 1;
    }
    log_info_fmt("reading map from: %s", path);

    MapInfo_t map_info;
    for (int i = 0; i < MAP_SIZE_TOTAL; i++) {
        const size_t num = fread(&map_info, sizeof(MapInfo_t), 1, fp);
        if (num != 1) {
            log_warning_fmt("could not read maol number: %d", i);
            fclose(fp);
            return 1;
        }
        if (!check_id(map_info.id)) {
            log_warning_fmt("invalid map id found: %d", i);
        }
        if (!check_variance(map_info.variance)) {
            log_warning_fmt("invalid variance found: %d", i);
        }
    }

    fclose(fp);
    return 0;
}
