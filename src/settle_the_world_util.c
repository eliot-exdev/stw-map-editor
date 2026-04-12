/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include "settle_the_world_util.h"
#include "ui_definitions.h"

#define EXDEVGFX2_LOG_LEVEL 1
#include <exdevgfx/logger.h>
#include <exdevgfx/helper.h>

#include <assert.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

void stw_read_tiles(Tiles8bit_t *tiles_map, Tiles8bit_t *tiles_map_coast, Tiles8bit_t *tiles_icon, Tiles8bit_t *tiles_bonus, Framebuffer8Bit_t *colors_mini_map) {
    int res = 0;

    // read framebuffer
    Framebuffer8Bit_t fb;
    res = framebuffer_8bit_read_from_dat(&fb, "assets/tiles_8bit.dat");
    if (res) {
        log_warning("could not read assets/tiles_8bit.dat");
        return;
    }

    // get all tiles
    Tiles8bit_t all_tiles;
    tiles_8bit_init_from_framebuffer(&all_tiles, &fb, TILE_WIDTH, TILE_HEIGHT);
    framebuffer_8bit_deinit(&fb);

    // extract map tiles
    for (int i = 0; i < TILES_MAP_NUM; ++i) {
        framebuffer_8bit_copy_to(all_tiles.tiles + i, tiles_map->tiles + i);
    }

    // extract map coast tiles
    for (int i = 0; i < TILES_MAP_COAST_NUM; ++i) {
        framebuffer_8bit_copy_to(all_tiles.tiles + i + TILES_MAP_NUM, tiles_map_coast->tiles + i);
    }

    // extract icon tiles
    for (int i = 0; i < TILES_ICON_NUM; ++i) {
        framebuffer_8bit_copy_to(all_tiles.tiles + i + TILES_MAP_NUM + TILES_MAP_COAST_NUM, tiles_icon->tiles + i);
    }

    // extract colors mini map
    framebuffer_8bit_copy_to(all_tiles.tiles + TILES_MAP_NUM + TILES_MAP_COAST_NUM + TILES_ICON_NUM, colors_mini_map);

    // extract bonus
    for (int i = 0; i < TILES_BONUS_NUM; ++i) {
        framebuffer_8bit_copy_to(all_tiles.tiles + i + TILES_MAP_NUM + TILES_MAP_COAST_NUM + TILES_ICON_NUM + 1 + TILES_UNUSED_NUM, tiles_bonus->tiles + i);
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

enum BONUS_IDS {
    BONUS_ID_NONE = 0,
    BONUS_ID_GOLD,
    BONUS_ID_GRAPES,
    BONUS_ID_GRAIN,
    BONUS_ID_SUGAR,
    BONUS_ID_WOOL,
    BONUS_ID_TABACCO,
    BONUS_ID_COFFEE_BEANS,
    BONUS_ID_FURS,
    BONUS_ID_CRAPS,
    BONUS_ID_LUMBER,
    BONUS_ID_ORE,
    BONUS_ID_SHIP_WRECK_1,
    BONUS_ID_SHIP_WRECK_2,
    BONUS_ID_ABANDONED_SETTLEMENT_1,
    BONUS_ID_ABANDONED_SETTLEMENT_2
};

static enum BONUS_IDS to_bonus_id(const uint8_t id) {
    switch (id) {
        case 15:
            return BONUS_ID_GOLD;
        case 17:
            return BONUS_ID_GRAPES;
        case 1:
            return BONUS_ID_GRAIN;
        case 6:
            return BONUS_ID_SUGAR;
        case 4:
            return BONUS_ID_WOOL;
        case 7:
            return BONUS_ID_TABACCO;
        case 8:
            return BONUS_ID_COFFEE_BEANS;
        case 5:
            return BONUS_ID_FURS;
        case 3:
            return BONUS_ID_CRAPS;
        case 11:
            return BONUS_ID_LUMBER;
        case 12:
            return BONUS_ID_ORE;
        case 19:
            return BONUS_ID_SHIP_WRECK_1;
        case 20:
            return BONUS_ID_ABANDONED_SETTLEMENT_1;
        case 21:
            return BONUS_ID_SHIP_WRECK_2;
        case 22:
            return BONUS_ID_ABANDONED_SETTLEMENT_2;
    }
    return BONUS_ID_NONE;
}

static uint8_t from_bonus_id(const int id) {
    switch (id) {
        case BONUS_ID_GOLD:
            return 15;
        case BONUS_ID_GRAPES:
            return 17;
        case BONUS_ID_GRAIN:
            return 1;
        case BONUS_ID_SUGAR:
            return 6;
        case BONUS_ID_WOOL:
            return 4;
        case BONUS_ID_TABACCO:
            return 7;
        case BONUS_ID_COFFEE_BEANS:
            return 8;
        case BONUS_ID_FURS:
            return 5;
        case BONUS_ID_CRAPS:
            return 3;
        case BONUS_ID_LUMBER:
            return 11;
        case BONUS_ID_ORE:
            return 12;
        case BONUS_ID_SHIP_WRECK_1:
            return 19;
        case BONUS_ID_SHIP_WRECK_2:
            return 21;
        case BONUS_ID_ABANDONED_SETTLEMENT_1:
            return 20;
        case BONUS_ID_ABANDONED_SETTLEMENT_2:
            return 22;
    }
    return 0;
}

static void update_map_info(MapInfo_t *info, const MapTile_t *tile) {
    switch (tile->tile_id) {
        case 0:
            info->id = 0;
            info->variance = 1;
            break;
        case 1:
            info->id = 0;
            info->variance = 2;
            break;
        case 2:
            info->id = 0;
            info->variance = 3;
            break;
        case 3:
            info->id = 1;
            info->variance = 1;
            break;
        case 4:
            info->id = 1;
            info->variance = 2;
            break;
        case 5:
            info->id = 1;
            info->variance = 3;
            break;
        case 6:
            info->id = 3;
            info->variance = 1;
            break;
        case 7:
            info->id = 3;
            info->variance = 2;
            break;
        case 8:
            info->id = 3;
            info->variance = 3;
            break;
        case 9:
            info->id = 4;
            info->variance = 1;
            break;
        case 10:
            info->id = 4;
            info->variance = 2;
            break;
        case 11:
            info->id = 4;
            info->variance = 3;
            break;
        case 12:
            info->id = 5;
            info->variance = 1;
            break;
        case 13:
            info->id = 5;
            info->variance = 2;
            break;
        case 14:
            info->id = 5;
            info->variance = 3;
            break;
        case 15:
            info->id = 6;
            info->variance = 1;
            break;
        case 16:
            info->id = 6;
            info->variance = 2;
            break;
        case 17:
            info->id = 6;
            info->variance = 3;
            break;
        case 18:
            info->id = 7;
            info->variance = 1;
            break;
        case 19:
            info->id = 7;
            info->variance = 2;
            break;
        case 20:
            info->id = 7;
            info->variance = 3;
            break;
        case 21:
            info->id = 8;
            info->variance = 1;
            break;
        case 22:
            info->id = 8;
            info->variance = 2;
            break;
        case 23:
            info->id = 8;
            info->variance = 3;
            break;
        case 24:
            info->id = 9;
            info->variance = 1;
            break;
        case 25:
            info->id = 9;
            info->variance = 2;
            break;
        case 26:
            info->id = 9;
            info->variance = 3;
            break;
        case 27:
            info->id = 10;
            info->variance = 1;
            break;
        case 28:
            info->id = 10;
            info->variance = 2;
            break;
        case 29:
            info->id = 10;
            info->variance = 3;
            break;
        case 30:
            info->id = 11;
            info->variance = 1;
            break;
        case 31:
            info->id = 11;
            info->variance = 2;
            break;
        case 32:
            info->id = 11;
            info->variance = 3;
            break;
        case 33:
            info->id = 12;
            info->variance = 1;
            break;
        case 34:
            info->id = 12;
            info->variance = 2;
            break;
        case 35:
            info->id = 12;
            info->variance = 3;
            break;
        case 36:
            info->id = 13;
            info->variance = 1;
            break;
        case 37:
            info->id = 13;
            info->variance = 2;
            break;
        case 38:
            info->id = 13;
            info->variance = 3;
            break;
        case 39:
            info->id = 14;
            info->variance = 1;
            break;
        case 40:
            info->id = 14;
            info->variance = 2;
            break;
        case 41:
            info->id = 14;
            info->variance = 3;
            break;
        case 42:
            info->id = 15;
            info->variance = 1;
            break;
        case 43:
            info->id = 15;
            info->variance = 2;
            break;
        case 44:
            info->id = 15;
            info->variance = 3;
            break;
        case 45:
            info->id = 16;
            info->variance = 1;
            break;
        case 46:
            info->id = 16;
            info->variance = 2;
            break;
        case 47:
            info->id = 16;
            info->variance = 3;
            break;
        case 48:
            info->id = 17;
            info->variance = 1;
            break;
        case 49:
            info->id = 17;
            info->variance = 2;
            break;
        case 50:
            info->id = 17;
            info->variance = 3;
            break;
        case 51:
            info->id = 18;
            info->variance = 1;
            break;
        case 52:
            info->id = 18;
            info->variance = 2;
            break;
        case 53:
            info->id = 18;
            info->variance = 3;
            break;
        case 54:
            info->id = 19;
            info->variance = 1;
            break;
        case 55:
            info->id = 19;
            info->variance = 2;
            break;
        case 56:
            info->id = 19;
            info->variance = 3;
            break;
        case 57:
            info->id = 20;
            info->variance = 1;
            break;
        case 58:
            info->id = 20;
            info->variance = 2;
            break;
        case 59:
            info->id = 20;
            info->variance = 3;
            break;
        case 60:
            info->id = 21;
            info->variance = 1;
            break;
        case 61:
            info->id = 21;
            info->variance = 2;
            break;
        case 62:
            info->id = 21;
            info->variance = 3;
            break;
        case 63:
            info->id = 22;
            info->variance = 1;
            break;
        case 64:
            info->id = 22;
            info->variance = 2;
            break;
        case 65:
            info->id = 22;
            info->variance = 3;
            break;
        case 66:
            info->id = 23;
            info->variance = 1;
            break;
        case 67:
            info->id = 23;
            info->variance = 2;
            break;
        case 68:
            info->id = 23;
            info->variance = 3;
            break;
        case 69:
            info->id = 24;
            info->variance = 1;
            break;
        case 70:
            info->id = 24;
            info->variance = 2;
            break;
        case 71:
            info->id = 24;
            info->variance = 3;
            break;
        case 72:
            info->id = 25;
            info->variance = 1;
            break;
        case 73:
            info->id = 25;
            info->variance = 2;
            break;
        case 74:
            info->id = 25;
            info->variance = 3;
            break;
        case 75:
            info->id = 26;
            info->variance = 1;
            break;
        case 76:
            info->id = 26;
            info->variance = 2;
            break;
        case 77:
            info->id = 26;
            info->variance = 3;
            break;
        case 78:
            info->id = 27;
            info->variance = 1;
            break;
        case 79:
            info->id = 27;
            info->variance = 2;
            break;
        case 80:
            info->id = 27;
            info->variance = 3;
            break;
        case 81:
            info->id = 28;
            info->variance = 1;
            break;
        case 82:
            info->id = 28;
            info->variance = 2;
            break;
        case 83:
            info->id = 28;
            info->variance = 3;
            break;
        case 84:
            info->id = 29;
            info->variance = 1;
            break;
        case 85:
            info->id = 29;
            info->variance = 2;
            break;
        case 86:
            info->id = 29;
            info->variance = 3;
            break;
        case 87:
            info->id = 32;
            info->variance = 1;
            break;
        case 88:
            info->id = 32;
            info->variance = 2;
            break;
        case 89:
            info->id = 32;
            info->variance = 3;
            break;
        case 90:
            info->id = 33;
            info->variance = 1;
            break;
        case 91:
            info->id = 33;
            info->variance = 2;
            break;
        case 92:
            info->id = 33;
            info->variance = 3;
            break;
        case 93:
            info->id = 34;
            info->variance = 1;
            break;
        case 94:
            info->id = 34;
            info->variance = 2;
            break;
        case 95:
            info->id = 34;
            info->variance = 3;
            break;
        default:
            log_warning_fmt("could not convert id: %d to MapInfo", (int) tile->tile_id);
            break;
    }

    info->object_id = from_bonus_id(tile->bonus_id);
}

static int check_id(const uint8_t id) {
    return id < 35;
}
static int check_variance(const uint8_t variance) {
    return variance > 0 && variance < 4;
}

static uint8_t to_tile_id(const enum MAP_TILE_IDS id, const uint8_t variance) {
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

int stw_is_ocean_tile(const int id) {
    return id == MAP_TILE_ID_OCEAN || id == MAP_TILE_ID_OCEAN + 1 || id == MAP_TILE_ID_OCEAN + 2;
}

uint8_t stw_get_shore_tile_id_straight(const int id, const uint8_t sum) {
    switch (sum) {
        case 1:
            return 106 -TILES_MAP_NUM;
        case 2:
            return 127 - TILES_MAP_NUM;
        case 3:
            return 107 - TILES_MAP_NUM;
        case 4:
            return 146 - TILES_MAP_NUM;
        case 5:
            return 102 - TILES_MAP_NUM;
        case 6:
            return 147 - TILES_MAP_NUM;
        case 7:
            return 103 - TILES_MAP_NUM;
        case 8:
            return 125 - TILES_MAP_NUM;
        case 9:
            return 105 - TILES_MAP_NUM;
        case 10:
            return 120 - TILES_MAP_NUM;
        case 11:
            return 100 - TILES_MAP_NUM;
        case 12:
            return 145 - TILES_MAP_NUM;
        case 13:
            return 101 - TILES_MAP_NUM;
        case 14:
            return 140 - TILES_MAP_NUM;
        case 15:
            return 104 - TILES_MAP_NUM;
    }
    return id;
}

uint8_t stw_get_shore_tile_id_angular(const int id, const uint8_t sum) {
    switch (sum) {
        case 16:
            return 153 - TILES_MAP_NUM;
        case 32:
            return 154 - TILES_MAP_NUM;
        case 64:
            return 111 - TILES_MAP_NUM;
        case 128:
            return 112 - TILES_MAP_NUM;
        case 48:
            return 157 - TILES_MAP_NUM;
        case 96:
            return 158 - TILES_MAP_NUM;
        case 192:
            return 163 - TILES_MAP_NUM;
        case 144:
            return 156 - TILES_MAP_NUM;
        case 160:
            return 132 - TILES_MAP_NUM;
        case 80:
            return 131 - TILES_MAP_NUM;
        case 208:
            return 119 - TILES_MAP_NUM;
        case 176:
            return 159 - TILES_MAP_NUM;
        case 240:
            return 139 - TILES_MAP_NUM;
        case 224:
            return 165 - TILES_MAP_NUM;
        case 112:
            return 164 - TILES_MAP_NUM;
    }
    return id;
}

int stw_write_map(const char *orig_path, const MapTileArray_t(map)) {
    assert(orig_path);
    assert(map);

    // open in file
    FILE *in = fopen(orig_path, "r");
    if (!in) {
        log_warning_fmt("could not open map from path: %s", orig_path);
        return 1;
    }

    // open out file
    char dst_path[256];
    memset(dst_path, '0', 256);
    sprintf(dst_path, "%s.mod", orig_path);
    FILE *out = fopen(dst_path, "w");
    if (!out) {
        log_warning_fmt("could not open map from path: %s", dst_path);
        return 2;
    }
    log_info_fmt("writing map to: %s", dst_path);

    MapInfo_t map_info;
    int x = 0;
    int y = 0;

    for (int i = 0; i < MAP_SIZE_TOTAL; i++) {
        // read
        const size_t r_num = fread(&map_info, sizeof(MapInfo_t), 1, in);
        if (r_num != 1) {
            log_warning_fmt("could not read map info number: %d", i);
            fclose(in);
            fclose(out);
            return 3;
        }

        // update
        update_map_info(&map_info, &map[y][x]);

        // write
        const size_t w_num = fwrite(&map_info, sizeof(MapInfo_t), 1, out);
        if (w_num != 1) {
            log_warning_fmt("could not write map info number: %d", i);
            fclose(in);
            fclose(out);
            return 4;
        }

        // update x & y
        ++x;
        if (x == MAP_SIZE_X) {
            ++y;
            x = 0;
        }
    }

    fclose(in);
    fclose(out);

    if (strstr(orig_path, ".mod")) {
        log_info("moving files");
        remove(orig_path);
        rename(dst_path, orig_path);
    }
    return 0;
}

int stw_write_new_map(const char *path, const MapTileArray_t(map)) {
    assert(path);
    assert(map);

    // open out file
    FILE *out = fopen(path, "w");
    if (!out) {
        log_warning_fmt("could not open map from path: %s", path);
        return 2;
    }
    log_info_fmt("writing new map to: %s", path);

    MapInfo_t map_info;
    int x = 0;
    int y = 0;

    for (int i = 0; i < MAP_SIZE_TOTAL; i++) {
        // update
        update_map_info(&map_info, &map[y][x]);

        // write
        const size_t w_num = fwrite(&map_info, sizeof(MapInfo_t), 1, out);
        if (w_num != 1) {
            log_warning_fmt("could not write new map info number: %d", i);
            fclose(out);
            return 4;
        }

        // update x & y
        ++x;
        if (x == MAP_SIZE_X) {
            ++y;
            x = 0;
        }
    }

    fclose(out);

    return 0;
}

int stw_read_map(const char *path, MapTileArray_t(map)) {
    assert(path);
    assert(map);

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
            log_warning_fmt("could not read map info number: %d", i);
            fclose(fp);
            return 1;
        }

        if (!check_id(map_info.id)) {
            log_warning_fmt("invalid map id found: %d", i);
        }

        if (!check_variance(map_info.variance)) {
            log_warning_fmt("invalid variance found: %d", i);
        }

        map_info.variance -= 1;// stw variance is from 1 to 3, but we use 0 to 2

        const int y = i / MAP_SIZE_X;
        const int x = i % MAP_SIZE_X;
        map[y][x].tile_id = to_tile_id(map_info.id, map_info.variance);
        map[y][x].bonus_id = to_bonus_id(map_info.object_id);
    }

    fclose(fp);
    return 0;
}

void stw_randomize_tile_variants(MapTileArray_t(map)) {
    assert(map);

    srand(now());

    MapInfo_t map_info;
    for (int y = 0; y < MAP_SIZE_Y; ++y) {
        for (int x = 0; x < MAP_SIZE_X; ++x) {
            update_map_info(&map_info, &map[y][x]);
            map_info.variance = rand() % 3;
            map[y][x].tile_id = to_tile_id(map_info.id, map_info.variance);
        }
    }
}

int stw_has_valid_bonus(const MapTile_t *tile) {
    assert(tile);

    if (tile->bonus_id == 0) {
        return 1;
    }

    if (stw_is_ocean_tile(tile->tile_id)) {
        return tile->bonus_id == BONUS_ID_SHIP_WRECK_1 || tile->bonus_id == BONUS_ID_SHIP_WRECK_2 || tile->bonus_id == BONUS_ID_CRAPS;
    }
    return tile->bonus_id != BONUS_ID_SHIP_WRECK_1 & tile->bonus_id != BONUS_ID_SHIP_WRECK_2 && tile->bonus_id != BONUS_ID_CRAPS;
}
