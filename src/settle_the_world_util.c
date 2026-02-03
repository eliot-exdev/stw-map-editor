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

// static int VALID_TILES[] = {
//         0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,          // row 1
//         20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,// row 2
//         40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,// row 3
//         60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,// row 4
//         80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95                 // row 5
// };

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
    const int total = MAP_TILES_NUM + MAP_TILES_COAST_NUM;
    tiles_8bit_init(tiles, total, TILE_WIDTH, TILE_HEIGHT);
    for (int i = 0; i < total; i++) {
        framebuffer_8bit_copy_to(all_tiles.tiles + i, tiles->tiles + i);
    }

    // cleanup
    tiles_8bit_deinit(&all_tiles);
}

enum MAP_TILE_IDS {
    MAP_TILE_ID_OCEAN = 0,
    MAP_TILE_ID_GRASS = 1,
    // MAP_TILE_ID_BEACH = 2,
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

static unsigned char to_tile_id(const enum MAP_TILE_IDS id, const unsigned char variance) {
    switch (id) {
        case MAP_TILE_ID_OCEAN:
            return 0 + variance;
        case MAP_TILE_ID_GRASS:
            return 3 + variance;
        case MAP_TILE_ID_PRAIRIE:
            return 6 + variance;
        case MAP_TILE_ID_DESERT:
            return 9 + variance;
        case MAP_TILE_ID_DRYLAND:
            return 12 + variance;
        case MAP_TILE_ID_WETLAND:
            return 15 + variance;
        case MAP_TILE_ID_SWAMP:
            return 18 + variance;
        case MAP_TILE_ID_SOIL:
            return 21 + variance;
        case MAP_TILE_ID_COLDLAND:
            return 24 + variance;
        case MAP_TILE_ID_TUNDRA:
            return 27 + variance;
        case MAP_TILE_ID_ICELAND:
            return 30 + variance;
        case MAP_TILE_ID_PLAINS:
            return 33 + variance;
        case MAP_TILE_ID_SAVANNAH:
            return 36 + variance;
        case MAP_TILE_ID_SALTPONDS:
            return 39 + variance;
        case MAP_TILE_ID_CLAYPIT:
            return 42 + variance;
        case MAP_TILE_ID_LMOUNTAIN:
            return 45 + variance;
        case MAP_TILE_ID_IRONHILLS:
            return 48 + variance;
        case MAP_TILE_ID_MMOUNTAIN:
            return 51 + variance;
        case MAP_TILE_ID_MARBLEMOUNTAIN:
            return 54 + variance;
        case MAP_TILE_ID_HMOUNTAIN:
            return 57 + variance;
        case MAP_TILE_ID_TMOUNTAIN:
            return 60 + variance;
        case MAP_TILE_ID_LAKE:
            return 63 + variance;
        case MAP_TILE_ID_GRASSFOREST:
            return 66 + variance;
        case MAP_TILE_ID_PRAIRIEFOREST:
            return 69 + variance;
        case MAP_TILE_ID_DESERTFOREST:
            return 72 + variance;
        case MAP_TILE_ID_DRYLANDFOREST:
            return 75 + variance;
        case MAP_TILE_ID_WETLANDFOREST:
            return 78 + variance;
        case MAP_TILE_ID_SWAMPFOREST:
            return 81 + variance;
        case MAP_TILE_ID_COLDLANDFOREST:
            return 84 + variance;
        case MAP_TILE_ID_LMOUNTAINFOREST:
            return 87 + variance;
        case MAP_TILE_ID_MMOUNTAINFOREST:
            return 90 + variance;
        case MAP_TILE_ID_TROPICALFOREST:
            return 93 + variance;
    }
    return 0;
}

int is_ocean_tile(const int id) {
    return id == MAP_TILE_ID_OCEAN || id == MAP_TILE_ID_OCEAN + 1 || id == MAP_TILE_ID_OCEAN + 2;
}

uint8_t get_shore_tile_id_straight(const int id, const uint8_t sum) {
    switch (sum) {
        case 1:
            return 106;
        case 2:
            return 127;
        case 3:
            return 107;
        case 4:
            return 146;
        case 5:
            return 102;
        case 6:
            return 147;
        case 7:
            return 103;
        case 8:
            return 125;
        case 9:
            return 105;
        case 10:
            return 120;
        case 11:
            return 100;
        case 12:
            return 145;
        case 13:
            return 101;
        case 14:
            return 140;
        case 15:
            return 104;
    }
    return id;
}

uint8_t get_shore_tile_id_angular(const int id, const uint8_t sum) {
    switch (sum) {
        case 16:
            return 153;
        case 32:
            return 154;
        case 64:
            return 111;
        case 128:
            return 112;
    }
    return id;
}

int stw_read_map(const char *path, unsigned char map[MAP_SIZE_Y][MAP_SIZE_X]) {
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
        map_info.variance -= 1;

        const int y = i / MAP_SIZE_X;
        const int x = i % MAP_SIZE_X;
        map[y][x] = to_tile_id(map_info.id, map_info.variance);
    }

    fclose(fp);
    return 0;
}
