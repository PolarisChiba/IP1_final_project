#include "scene_12p.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"
#include "utility.h"
#include "shared.h"
#include "scene_menu.h"
#include "scene_story.h"
#include "scene_practice.h"

static ALLEGRO_BITMAP *p1, *p2, *bag;
static int w = 300, h = 200;
static int px1 = (1920 - 2 * 300) / 3, py1 = 350;
static int px2 = (1920 - 2 * 300) * 2 / 3 + 200, py2 = 350;
static ALLEGRO_SAMPLE *depart;
static ALLEGRO_SAMPLE_ID depart_id;

static void init() {
	bag = load_bitmap("./images/background/staff.bmp");
	p1 = load_bitmap_resized("./images/button/p1.png", w, h);
	p2 = load_bitmap_resized("./images/button/p2.png", w, h);
	depart = load_audio("./bgm/depart.ogg");
	depart_id = play_audio(depart, 1);
}

static void draw() {
	al_draw_bitmap(bag, 0, 0, 0);
	al_draw_bitmap(p1, px1, py1, 0);
	al_draw_bitmap(p2, px2, py2, 0);
}

static void destroy() {
	al_destroy_bitmap(bag);
	al_destroy_bitmap(p1);
	al_destroy_bitmap(p2);
	al_destroy_sample(depart);
	stop_bgm(depart_id);
}

static void on_mouse_down(int btn, int x, int y, int dz) {
	if (btn == 1) {
		if (pnt_in_rect(x, y, px1, py1, w, h)) {
			player_num = 1;
			game_change_scene(scene_practice_create());
		}
		else if (pnt_in_rect(x, y, px2, py2, w, h)) {
			player_num = 2;
			game_change_scene(scene_practice_create());
		}
	}
}

static void on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_BACKSPACE:
			game_change_scene(scene_menu_create());
			break;
	}
}

Scene scene_12p_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Choose";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
    // TODO: Register more event callback functions such as update, mouse, ...
    game_log("Menu scene created");
    return scene;
}
