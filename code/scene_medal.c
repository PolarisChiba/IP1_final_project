#include "scene_settings.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"
#include <stdio.h>
#include <stdlib.h>

/* Define your static vars / function prototypes below. */

static ALLEGRO_BITMAP *background;
static ALLEGRO_SAMPLE *medal_s;
static ALLEGRO_SAMPLE_ID medal_id;

static void init() {
	background = load_bitmap_resized("./images/background/achievement.bmp", WINDOW_W, WINDOW_H);
	medal_s = load_audio("./bgm/medal.ogg");
	medal_id = play_bgm(medal_s, 1);
}

int cmpfunc (const void * a, const void * b) {
   return -( *(int*)a - *(int*)b );
}

static void draw(void) {
	qsort(history_score, h_sz, sizeof(int), cmpfunc);
	al_draw_bitmap(background, 0, 0, 0);
	int res = h_sz;
	if (10 < res) res = 10;
	for (int i = 0; i < 10; ++ i) {
		char score_num[20];
		score_num[0] = i + '0';
		score_num[1] = ':';
		score_num[2] = ' ';
		score_num[3] = ' ';
		if (i < h_sz) trans(score_num + 4, history_score[i]);
		else trans(score_num + 4, 0);
		if (i < 5) al_draw_text(font_72, al_map_rgb(255, 255, 255), 300, 200 + 140 * i, 0, score_num);
		else al_draw_text(font_72, al_map_rgb(255, 255, 255), 1100, 200 + 140 * (i - 5), 0, score_num);
	}
}

static void on_key_down(int keycode) {
    if (keycode == ALLEGRO_KEY_BACKSPACE) {
    	game_change_scene(scene_menu_create());
	}
}

static void destroy() {
	al_destroy_bitmap(background);
	al_destroy_sample(medal_s);
	stop_bgm(medal_id);
}

// The only function that is shared across files.
Scene scene_medal_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Medal";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.on_key_down = &on_key_down;
    scene.destroy = &destroy;
    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Medal scene created");
    return scene;
}
