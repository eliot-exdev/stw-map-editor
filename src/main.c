/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#include "settle_the_world_map_editor.h"
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
unsigned long __stack = (32768);// 32 kb
#endif

int main(int argc, char **argv) {
    parse_args(argc, argv);

    int res = exdev_base_init();
    if (res) {
        log_warning("could not init exdevgfx");
        return res;
    }

    STWMapEditor_t editor;// this is our usr_ptr
    if (argc > 1) {
        editor.map_path = argv[1];
        log_info_fmt("map path: %s", editor.map_path);
    } else {
        editor.map_path = NULL;
    }

    log_info("--> read tiles");
    Tiles8bit_t tiles;
    stw_read_tiles(&tiles);
    log_info("<-- read tiles");

    // setup application
    log_info("--> setup ui");
    UIApplication_t app;
    ui_application_init(&app, UI_WIDTH, UI_HEIGHT, &editor);
    res = palette_8bit_read_from_dat(&app.palette, "assets/maptiles_8bit.pal");
    if (res) {
        log_warning("could not read assets/maptiles_8bit.pal");
        return res;
    }

    // status
    editor.status = ui_status_create(UI_BORDER_SIZE, UI_BORDER_SIZE, UI_STATUS_WIDTH, UI_STATUS_HEIGHT, &tiles);
    ui_component_connect(&app.root, editor.status);

    // map component
    editor.map = ui_map_create(UI_BORDER_SIZE, UI_MAP_Y_POS, UI_MAP_WIDTH, UI_MAP_HEIGHT, &tiles);
    ui_component_connect(&app.root, editor.map);

    // tile view
    editor.tile = ui_tile_create(UI_TILE_X_POS, UI_BORDER_SIZE, UI_TILE_WIDTH, UI_TILE_HEIGHT, &tiles);
    ui_component_connect(&app.root, editor.tile);

    ui_application_prepare(&app);
    log_info("<-- setup ui");

    if (editor.map_path) {
        res = stw_read_map(editor.map_path);
        log_info_fmt("result read map: %d", res);
    }

    // run
    log_info("--> run");
    ui_application_run(&app, "stw_map_editor", UPDATE_INTERVAL);
    log_info("<-- run");

    // cleanup
    log_info("--> cleanup");
    ui_application_destroy(&app);
    tiles_8bit_deinit(&tiles);
    exdev_base_deinit();
    log_info("<-- cleanup");

    return 0;
}
