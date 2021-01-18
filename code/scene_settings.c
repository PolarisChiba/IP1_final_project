#include "scene_settings.h"
#include "scene_menu.h"
#include "utility.h"

/* Define your static vars / function prototypes below. */

static ALLEGRO_BITMAP *background;
static ALLEGRO_SAMPLE *setting_s;
static ALLEGRO_SAMPLE_ID setting_id;

static void init() {
	background = load_bitmap_resized("./images/background/setting.jpg", WINDOW_W, WINDOW_H);
	setting_s = load_audio("./bgm/setting.ogg");
	setting_id = play_bgm(setting_s, 1);
}


static void draw(void) {
	al_draw_bitmap(background, 0, 0, 0);
}

static void on_key_down(int keycode) {
    if (keycode == ALLEGRO_KEY_BACKSPACE) {
    	game_change_scene(scene_menu_create());
	}
}

static void destroy() {
	al_destroy_bitmap(background);
	al_destroy_sample(setting_s);
	stop_bgm(setting_id);
}

// The only function that is shared across files.
Scene scene_settings_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Settings";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.on_key_down = &on_key_down;
    scene.destroy = &destroy;
    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Settings scene created");
    return scene;
}
