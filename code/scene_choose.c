#include "scene_choose.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"
#include "utility.h"
#include "shared.h"
#include "scene_menu.h"
#include "scene_story.h"

static ALLEGRO_BITMAP *ch1, *ch2, *bag;
static int w = 200, h = 200;
static int px1 = (1920 - 2 * 200) / 3, py1 = 300;
static int px2 = (1920 - 2 * 200) * 2 / 3 + 200, py2 = 300;
static ALLEGRO_SAMPLE *depart;
static ALLEGRO_SAMPLE_ID depart_id;

static void init() {
	bag = load_bitmap("./images/background/staff.bmp");
	ch1 = load_bitmap_resized("./images/staff/plane.png", w, h);
	ch2 = load_bitmap_resized("./images/staff/tank-2.png", w, h);
	depart = load_audio("./bgm/depart.ogg");
	depart_id = play_audio(depart, 1);
}

static void draw() {
	al_draw_bitmap(bag, 0, 0, 0);
	al_draw_bitmap(ch1, px1, py1, 0);
	al_draw_bitmap(ch2, px2, py2, 0);
	al_draw_text(font_72, al_map_rgb(255, 255, 255), px1 + w / 2 - 100, py1 + 300, ALLEGRO_ALIGN_CENTER, "enemy bullet clear");
	al_draw_text(font_72, al_map_rgb(255, 255, 255), px2 + w / 2 + 100, py2 + 300, ALLEGRO_ALIGN_CENTER, "atk damage up 2.5");
}

static void destroy() {
	al_destroy_bitmap(bag);
	al_destroy_bitmap(ch1);
	al_destroy_bitmap(ch2);
	al_destroy_sample(depart);
	stop_bgm(depart_id);
}

static void on_mouse_down(int btn, int x, int y, int dz) {
	if (btn == 1) {
		if (pnt_in_rect(x, y, px1, py1, w, h)) {
			staff_name = 0;
			game_change_scene(scene_story_create());
		}
		else if (pnt_in_rect(x, y, px2, py2, w, h)) {
			staff_name = 1;
			game_change_scene(scene_story_create());
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

Scene scene_choose_create(void) {
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
