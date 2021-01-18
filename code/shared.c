// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
#include "scene_menu.h"

ALLEGRO_FONT* font_72;
ALLEGRO_FONT* font_60;
ALLEGRO_FONT* font_48;
ALLEGRO_FONT* font_32;
ALLEGRO_FONT* font_24;

void shared_init(void) {
	font_72 = load_font("./fonts/Caviar_Dreams_Bold.ttf", 72);
	font_60 = load_font("./fonts/Caviar_Dreams_Bold.ttf", 60);
	font_48 = load_font("./fonts/Caviar_Dreams_Bold.ttf", 48);
    font_32 = load_font("./fonts/Caviar_Dreams_Bold.ttf", 32);
    font_24 = load_font("./fonts/Caviar_Dreams_Bold.ttf", 24);
    
    game_change_scene(scene_menu_create());
}

void shared_destroy(void) {
	al_destroy_font(font_72);
	al_destroy_font(font_60);
	al_destroy_font(font_48);
    al_destroy_font(font_32);
    al_destroy_font(font_24);
}
