#include "engine.h"

void multiplayer_select_menu_render(actor *a);

multiplayer_select_menu *multiplayer_select_menu_init(multiplayer_select_menu *this)
{
    actor_init(&this->a, multiplayer_select_menu_render, NULL);

    this->selected_index = 0;

    static char *fnames[] = {"assets/1p_menu.png", "assets/1p_menu_hl.png", "assets/2p_menu.png", "assets/2p_menu_hl.png"};
    decal *decals[] = {&this->one_player_mode_menu_entry_non_hl, &this->one_player_mode_menu_entry_hl,
        &this->two_player_mode_menu_entry_non_hl, &this->two_player_mode_menu_entry_hl};
    int i, w, h;

    for (i = 0; i < sizeof(fnames)/sizeof(fnames[0]); i++)
    {
        SDL_Surface *img = IMG_Load(fnames[i]);

        if(!img){
            fprintf(stdout, "Error! Could not load %s\n", fnames[i]);
            exit(1);
        }

        eng.textures[i] = SDL_CreateTextureFromSurface(eng.renderer, img);
        w = img->w;
        h = img->h;

        decal_init(
                decals[i],
                SDL_CreateTextureFromSurface(eng.renderer, img),
                0,
                0,
                w,
                h);

        SDL_FreeSurface(img);
    }

    sprite_init(&this->one_player_mode_menu_entry, w, h, &this->one_player_mode_menu_entry_hl);
    this->one_player_mode_menu_entry.r[0] = 198;
    this->one_player_mode_menu_entry.r[1] = 266;
    sprite_init(&this->two_player_mode_menu_entry, w, h, &this->two_player_mode_menu_entry_non_hl);
    this->two_player_mode_menu_entry.r[0] = 198;
    this->two_player_mode_menu_entry.r[1] = 397;
    return this;
}

void multiplayer_select_menu_render(actor *a)
{
    multiplayer_select_menu *this = (multiplayer_select_menu *)a;
    sprite_render(&this->one_player_mode_menu_entry);
    sprite_render(&this->two_player_mode_menu_entry);
}

PLAY_MODE multiplayer_select_menu_get_value(multiplayer_select_menu *this)
{
    return (this->selected_index == 0 ? PLAY_MODE_SP : PLAY_MODE_2P);
}

multiplayer_select_menu *multiplayer_select_menu_mv_up(multiplayer_select_menu *this)
{
    this->selected_index += 1;
    this->selected_index = this->selected_index % 2;

    if (this->selected_index == 0)
    {
        sprite_set_decal(&this->one_player_mode_menu_entry, &this->one_player_mode_menu_entry_hl);
        sprite_set_decal(&this->two_player_mode_menu_entry, &this->two_player_mode_menu_entry_non_hl);
    }
    else
    {
        sprite_set_decal(&this->one_player_mode_menu_entry, &this->one_player_mode_menu_entry_non_hl);
        sprite_set_decal(&this->two_player_mode_menu_entry, &this->two_player_mode_menu_entry_hl);
    }
    return this;
}


multiplayer_select_menu *multiplayer_select_menu_mv_down(multiplayer_select_menu *this)
{
    this->selected_index -= 1;
    this->selected_index = this->selected_index % 2;

    if (this->selected_index == 0)
    {
        sprite_set_decal(&this->one_player_mode_menu_entry, &this->one_player_mode_menu_entry_hl);
        sprite_set_decal(&this->two_player_mode_menu_entry, &this->two_player_mode_menu_entry_non_hl);
    }
    else
    {
        sprite_set_decal(&this->one_player_mode_menu_entry, &this->one_player_mode_menu_entry_non_hl);
        sprite_set_decal(&this->two_player_mode_menu_entry, &this->two_player_mode_menu_entry_hl);
    }
    return this;
}
