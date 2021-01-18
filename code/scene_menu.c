#include "scene_menu.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"
#include "utility.h"
#include "shared.h"
#include "scene_story.h"
#include "scene_practice.h"
#include "scene_settings.h"
#include "scene_choose.h"
#include "scene_12p.h"
#include "scene_medal.h"

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static const char* txt_title = "Aliens are invading";
static ALLEGRO_BITMAP* img_background;

static ALLEGRO_BITMAP* img_settings;
static ALLEGRO_BITMAP* img_settings2;

static ALLEGRO_SAMPLE_ID bgm_id;

static void init(void);
static void draw(void);
static void destroy(void);
static void on_key_down(int keycode);

static ALLEGRO_BITMAP* show_operator;

static ALLEGRO_BITMAP* story_button;
static ALLEGRO_BITMAP* death_match_button;
static ALLEGRO_BITMAP* medal_button;
static ALLEGRO_BITMAP* exit_button;

static void init(void) {
	al_show_mouse_cursor(game_display);
	
    img_background = load_bitmap_resized("./images/background/home.bmp", WINDOW_W, WINDOW_H);
    img_settings = al_load_bitmap("./images/button/settings.png");
    img_settings2 = al_load_bitmap("./images/button/settings2.png");
    // Can be moved to shared_init to decrease loading time.
    bgmg = load_audio("./bgm/home.ogg");
    if (ddd == 1)
    	bgmg = load_audio("./bgm/finish.ogg");
    if (ddd == 1)
    	txt_title = "May humans\' future bright";
    bgm_id = play_bgm(bgmg, 1);
    game_log("Menu scene initialized");
    
    
    story_button = load_bitmap_resized("./images/button/story.png", 250, 125);
	death_match_button = load_bitmap_resized("./images/button/practice.png", 250, 125);
    medal_button = load_bitmap_resized("./images/button/medal.png", 250, 125);
    exit_button = load_bitmap_resized("./images/button/exit.png", 250, 125);
}

static void draw(void) {
    al_draw_bitmap(img_background, 0, 0, 0);
    al_draw_bitmap(story_button, WINDOW_W - 300, 150, 0);
    al_draw_bitmap(death_match_button, WINDOW_W - 340, 350, 0);
    al_draw_bitmap(medal_button, WINDOW_W - 380, 550, 0);
    al_draw_bitmap(exit_button, WINDOW_W - 420, 750, 0);

    if (pnt_in_rect(mouse_x, mouse_y, WINDOW_W - 48, 10, 38, 38)) {
    	al_draw_bitmap(img_settings2, WINDOW_W - 48, 10, 0);
	}
	else {
		al_draw_bitmap(img_settings, WINDOW_W - 48, 10, 0);
	}

    al_draw_text(font_48, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, txt_title);
}

static void destroy(void) {
    al_destroy_bitmap(img_background);
    al_destroy_bitmap(img_settings);
    al_destroy_bitmap(img_settings2);
    stop_bgm(bgm_id);
    game_log("Menu scene destroyed");
}

static void on_key_down(int keycode) {
    if (keycode == ALLEGRO_KEY_ENTER)
        game_change_scene(scene_choose_create());
}

static void on_mouse_down(int btn, int x, int y, int dz) {
	if (btn == 1) {
		if (pnt_in_rect(x, y, WINDOW_W - 48, 10, 38, 38))
			game_change_scene(scene_settings_create());
	}
	
	if (btn == 1 && pnt_in_rect(x, y, WINDOW_W - 300, 150, 250, 125)) {
		game_change_scene(scene_choose_create());
	}
	if (btn == 1 && pnt_in_rect(x, y, WINDOW_W - 340, 350, 250, 125)) {
		game_change_scene(scene_12p_create());
	}
	if (btn == 1 && pnt_in_rect(x, y, WINDOW_W - 380, 550, 250, 125)) {
		game_change_scene(scene_medal_create());
	}
}

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_menu_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Menu";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
    // TODO: Register more event callback functions such as update, mouse, ...
    game_log("Menu scene created");
    return scene;
}

