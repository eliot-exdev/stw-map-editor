/**
* Copyright 2026 Andre Geisler (andre@exdev.de)
 */

#ifndef STW_MAP_EDITOR_UI_BONUS_H
#define STW_MAP_EDITOR_UI_BONUS_H
#include "ui_definitions.h"

struct UIBonus {
    UIComponent_t base;
};

typedef struct UIBonus UIBonus_t;

void ui_bonus_init(UIBonus_t *self, int x, int y, int width, int height, const Tiles8bit_t *bonuss);

UIBonus_t *ui_bonus_create(int x, int y, int width, int height, const Tiles8bit_t *bonuss);

void ui_bonus_destroy(UIBonus_t *self);

void ui_bonus_on_icon_click(UIIcon_t *icon, UIApplication_t *app, void *usr_ptr);

void ui_bonus_on_click(UIBonus_t *self, UIApplication_t *app, void *usr_ptr);

#endif //STW_MAP_EDITOR_UI_BONUS_H
