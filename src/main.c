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

#define VERSION "stw_me 0.1 (20.02.2026)"

#ifndef __linux__
#ifdef __VBCC__
__entry
#endif
        unsigned char versiontag[] = "\0$VER: " VERSION;
#endif

#if defined(__MORPHOS__) || defined(__AMIGA__)
#ifdef __VBCC__
__entry size_t __stack = 32768;// 32 kb
#else
size_t __stack = 32768;// 32 kb
#endif
#endif

static STWMapEditor_t editor;// this is our usr_ptr

static void print_help() {
    printf("stw_map_editor [ARGUMENTS]...\n"
           "arguments:\n"
           " -m, --map <path to map>  give a path to an existing map\n"
           " -h, --help               print help message and exit\n"
           " -v, --version            show version and exit\n");
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
    const char *ptr = NULL;
    if ((ptr = args_get_option_parameter(argc, argv, 'm', "map")) != NULL) {
        editor.map_path = ptr;
    }
}

int main(int argc, char **argv) {
    editor.map_path = NULL;

    parse_args(argc, argv);

    int res = exdev_base_init();
    if (res) {
        log_warning("could not init exdevgfx");
        return res;
    }

    log_info("--> read tiles");
    Tiles8bit_t tiles;
    stw_read_tiles(&tiles);
    log_info("<-- read tiles");

    // setup application
    log_info("--> setup ui");
    UIApplication_t app;
    ui_application_init(&app, UI_WIDTH, UI_HEIGHT, &editor);
    res = palette_8bit_read_from_dat(&app.palette, "assets/tiles_8bit.pal");
    if (res) {
        log_warning("could not read assets/tiles_8bit.pal");
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
