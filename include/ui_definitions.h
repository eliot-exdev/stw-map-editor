/**
 * Copyright 2025 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_UI_DEFINITIONS_H
#define STW_MAP_EDITOR_UI_DEFINITIONS_H

#include <exdevgfx/ui/ui.h>

#ifdef LOW_RESOLUTION

#define UI_WIDTH 320
#define UI_HEIGHT 256

#define UI_BORDER_SIZE 2

#define UI_STATUS_WIDTH 241
#define UI_STATUS_HEIGHT 20

#define UI_MAP_WIDTH 241
#define UI_MAP_HEIGHT 230
#define UI_MAP_Y_POS 24

#define UI_TILE_WIDTH 74
#define UI_TILE_HEIGHT 252
#define UI_TILE_X_POS 244
#else
#define UI_WIDTH 640
#define UI_HEIGHT 480

#define UI_BORDER_SIZE 2

#define UI_STATUS_WIDTH 561
#define UI_STATUS_HEIGHT 20

#define UI_MAP_WIDTH 561
#define UI_MAP_HEIGHT 454
#define UI_MAP_Y_POS 24

#define UI_TILE_WIDTH 74
#define UI_TILE_HEIGHT 476
#define UI_TILE_X_POS 564
#endif

#define UPDATE_INTERVAL 50// ms

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define UI_MAP_SUBTYPE 1
#define UI_TILE_SUBTYPE 2

#endif//STW_MAP_EDITOR_UI_DEFINITIONS_H
