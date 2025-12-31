/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include "ui_map.h"
#include "ui_tile.h"
#include "settle_the_world_util.h"

#include <exdevgfx/exdev_base.h>

#define EXDEVGFX2_LOG_LEVEL 2
#include <exdevgfx/logger.h>
#include <exdevgfx/args.h>

#include <stdlib.h>
#include <stdio.h>

#define VERSION "stw_map_editor 0.1 (29.12.2025)"

#ifndef __linux__
#ifdef __VBCC__
__entry
#endif
        unsigned char versiontag[] = "\0$VER: " VERSION;
#endif

static void print_help() {
    printf("stw_map_editor [ARGUMENTS]...\n"
           "arguments:\n"
           " -h, --help              print help message and exit\n"
           " -w, --world <number>    select world 1, 2, 3, 4 or 5 (default 1)\n");
}

static void print_version() {
    printf(VERSION "\n");
}

static void parse_args(int argc, char **argv) {
    if (args_find_option(argc, argv, 'h', "help")) {
        print_help();
        exit(0);
    }
    if (args_find_option(argc, argv, 'v', "version")) {
        print_version();
        exit(0);
    }
}

#if defined(__MORPHOS__) || defined(__AMIGAOS__)
unsigned long __stack = (16384);// 16 kb
#endif

int main(int argc, char **argv) {
    parse_args(argc, argv);

    int res = exdev_base_init();
    if (res) {
        log_warning("could not init exdevgfx");
        return res;
    }

    log_info("--> read tiles");
    Tiles8bit_t *tiles = stw_read_tiles();
    log_info("<-- read tiles");

    // setup application
    log_info("--> setup ui");
    UIApplication_t app;
    ui_application_init(&app, UI_WIDTH, UI_HEIGHT);
    res = palette_8bit_read_from_dat(&app.palette, "assets/maptiles_8bit.pal");
    if (res) {
        log_warning("could not read assets/maptiles_8bit.pal");
        return res;
    }

    // status
    UIComponent_t *status = ui_component_create(UI_BORDER_SIZE, UI_BORDER_SIZE, UI_STATUS_WIDTH, UI_STATUS_HEIGHT);
    ui_component_connect(&app.root, status);

    // map component
    UIMap_t *map_component = ui_map_create(UI_BORDER_SIZE, UI_MAP_Y_POS, UI_MAP_WIDTH, UI_MAP_HEIGHT, tiles);
    ui_component_connect(&app.root, map_component);

    // tile view
    UITile_t *tile_view = ui_tile_create(UI_TILE_X_POS, UI_BORDER_SIZE, UI_TILE_WIDTH, UI_TILE_HEIGHT, tiles, &map_component->properties.current_tile_index);
    ui_component_connect(&app.root, tile_view);

    ui_application_prepare(&app);
    log_info("<-- setup ui");

    // run
    log_info("--> run");
    ui_application_run(&app, "stw_map_editor", UPDATE_INTERVAL);
    log_info("<-- run");

    // cleanup
    log_info("--> cleanup");
    ui_application_destroy(&app);
    exdev_base_deinit();
    log_info("<-- cleanup");

    return 0;
}
