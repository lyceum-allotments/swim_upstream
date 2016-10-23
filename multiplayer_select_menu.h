#ifndef MULTIPLAYER_SELECT_MENU_H
#define MULTIPLAYER_SELECT_MENU_H
#include "engine.h"

typedef struct multiplayer_select_menu multiplayer_select_menu;

struct multiplayer_select_menu {
    actor a;
    sprite one_player_mode_menu_entry;
    decal one_player_mode_menu_entry_non_hl;
    decal one_player_mode_menu_entry_hl;
    
    sprite two_player_mode_menu_entry;
    decal two_player_mode_menu_entry_non_hl;
    decal two_player_mode_menu_entry_hl;

    unsigned int selected_index;
};

multiplayer_select_menu *multiplayer_select_menu_init(multiplayer_select_menu *this);
multiplayer_select_menu *multiplayer_select_menu_mv_up(multiplayer_select_menu *this);
multiplayer_select_menu *multiplayer_select_menu_mv_down(multiplayer_select_menu *this);
#endif
