#include "scene_story.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "game.h"
#include "utility.h"
#include "shared.h"
#include "scene_menu.h"
#include <math.h>

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static ALLEGRO_BITMAP* img_background, *right_background;
static ALLEGRO_BITMAP* img_plane;
static ALLEGRO_BITMAP* img_enemy;
static ALLEGRO_BITMAP* img_bullet;

static void init(void);
static void update(void);
static void draw_movable_object(MovableObject obj);
static void draw(void);
static void destroy(void);

#define MAX_ENEMY 3
#define MAX_BULLET 40
static MovableObject plane, planf;
static MovableObject enemies[MAX_ENEMY];
static MovableObject bullets[MAX_BULLET];

static const float MAX_COOLDOWN = 0.2f;
static double last_shoot_timestamp;
static double last_shoot_timestamf;
static ALLEGRO_SAMPLE* bgm;
static ALLEGRO_SAMPLE_ID bgm_id;
static bool draw_gizmos;

static double last_enemy;
static int kills;
static int lives;
static double last_die;

static void init(void) {
	srand(time(NULL));
	{
		kills = 0;
		lives = 0;
		last_die = -10;
	}
	
	right_background = load_bitmap_resized("./images/background/start-bg.jpg", WINDOW_W - SCREEN_W, WINDOW_H);
    img_background = load_bitmap_resized("./images/background/chapter_one.bmp", SCREEN_W, SCREEN_H);
    {
    	img_plane = plane.img = load_bitmap_resized("./images/staff/plane.png", 75, 75);
	    plane.w = al_get_bitmap_width(plane.img);
	    plane.h = al_get_bitmap_height(plane.img);
	    plane.x = SCREEN_W / 2 - plane.w / 2;
	    plane.y = SCREEN_H - plane.h * 3;
	    plane.vx = plane.vy = 3;
	    plane.hp = 100;
	    plane.hidden = false;
	    
	    planf.hidden = true;
	    planf.hp = 0;
	    if (player_num == 2) {
	    	planf = plane;
		    planf.x -= 200;
		    plane.x += 200;
		}
	}
    {
    	img_enemy = load_bitmap("./images/enemy/training_plane.png");
	    for (int i = 0; i < MAX_ENEMY; i++) {
	        enemies[i].img = img_enemy;
	        enemies[i].w = al_get_bitmap_width(img_enemy);
	        enemies[i].h = al_get_bitmap_height(img_enemy);
	        enemies[i].hidden = true;
		}
	}
	{
		img_bullet = al_load_bitmap("./images/bullets/bullet_1.png");
	    for (int i = 0; i < MAX_BULLET; ++ i) {
			bullets[i].img = img_bullet;
			bullets[i].w = al_get_bitmap_width(img_bullet);
			bullets[i].h = al_get_bitmap_height(img_bullet);
			bullets[i].vx = 0;
			bullets[i].vy = -30;
			bullets[i].hidden = true;
		}
	}
    
    // Can be moved to shared_init to decrease loading time.
    bgm = load_audio("./bgm/battle.ogg");
    game_log("Start scene initialized");
    bgm_id = play_bgm(bgm, 1);
}

static void update(void) {
	
	/*game over*/
	double die_now = al_get_time();
	if (plane.hp <= 0) {
		plane.hidden = true;
	}
	if (planf.hp <= 0) {
		planf.hidden = true;
	}
	
	/*plane*/ 
	plane.vx = plane.vy = 0;
    float plane_v = 5;
    if (key_state[ALLEGRO_KEY_UP])
        plane.vy -= plane_v;
    if (key_state[ALLEGRO_KEY_DOWN])
        plane.vy += plane_v;
    if (key_state[ALLEGRO_KEY_LEFT])
        plane.vx -= plane_v;
    if (key_state[ALLEGRO_KEY_RIGHT])
        plane.vx += plane_v;

    plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
    plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);

	if (plane.x < plane.w / 2)
		plane.x = plane.w / 2;
	else if (plane.x + plane.w / 2 > SCREEN_W)
		plane.x = SCREEN_W - plane.w / 2;
	if (plane.y < plane.h / 2)
		plane.y = plane.h / 2;
	else if (plane.y + plane.h / 2 > SCREEN_H)
		plane.y = SCREEN_H - plane.h / 2;
		
	planf.vx = planf.vy = 0;
    float planf_v = 5;
    if (key_state[ALLEGRO_KEY_W])
        planf.vy -= planf_v;
    if (key_state[ALLEGRO_KEY_S])
        planf.vy += planf_v;
    if (key_state[ALLEGRO_KEY_A])
        planf.vx -= planf_v;
    if (key_state[ALLEGRO_KEY_D])
        planf.vx += planf_v;

    planf.y += planf.vy * 4 * (planf.vx ? 0.71f : 1);
    planf.x += planf.vx * 4 * (planf.vy ? 0.71f : 1);

	if (planf.x < planf.w / 2)
		planf.x = planf.w / 2;
	else if (planf.x + planf.w / 2 > SCREEN_W)
		planf.x = SCREEN_W - planf.w / 2;
	if (planf.y < planf.h / 2)
		planf.y = planf.h / 2;
	else if (planf.y + planf.h / 2 > SCREEN_H)
		planf.y = SCREEN_H - planf.h / 2;
    
	/*bullet*/
    for (int i = 0; i < MAX_BULLET; ++ i) {
		if (bullets[i].hidden == true)
			continue;
		bullets[i].x += bullets[i].vx;
		bullets[i].y += bullets[i].vy;
		if (bullets[i].y < 0)
			bullets[i].hidden = true;
	}

	double now = al_get_time();
	if (key_state[ALLEGRO_KEY_RCTRL] && plane.hidden == false && now - last_shoot_timestamp >= MAX_COOLDOWN) {
		for (int i = 0; i < MAX_BULLET; ++ i) {
			if (bullets[i].hidden == true) {
				last_shoot_timestamp = now;
				bullets[i].hidden = false;
				bullets[i].x = plane.x;
				bullets[i].y = plane.y;
				break;
			}
		}
	}
	
	if (key_state[ALLEGRO_KEY_LCTRL] && planf.hidden == false && now - last_shoot_timestamf >= MAX_COOLDOWN) {
		for (int i = 0; i < MAX_BULLET; ++ i) {
			if (bullets[i].hidden == true) {
				last_shoot_timestamf = now;
				bullets[i].hidden = false;
				bullets[i].x = planf.x;
				bullets[i].y = planf.y;
				break;
			}
		}
	}

	/*enemy*/
	double enemy_now = al_get_time();
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[i].hidden == true && enemy_now - last_enemy >= 0.5f) {
			enemies[i].hidden = false;
			enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
	        enemies[i].y = 0;
	        enemies[i].vx = fmod((float)rand(), 5) + 1;
	    	enemies[i].vy = 5;
	    	
	    	last_enemy = enemy_now;
	    	break;
		}
	}
	
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[i].hidden == true) 
			continue;
		enemies[i].x += enemies[i].vx * cos(enemies[i].y / 100);
		enemies[i].y += enemies[i].vy;
		if (enemies[i].x < enemies[i].w / 2) 
			enemies[i].x = enemies[i].w / 2;
		else if (enemies[i].x > SCREEN_W - enemies[i].w / 2)
			enemies[i].x = SCREEN_W - enemies[i].w / 2;
		if (enemies[i].y > SCREEN_H) {
			enemies[i].hidden = true;
		}
	}
	
	/*collide, bullet, enemy*/ 
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[i].hidden == true) continue;
		for (int j = 0; j < MAX_BULLET; ++ j) {
			if (bullets[j].hidden == true) continue;
			if (collide(enemies[i], bullets[j])) {
				enemies[i].hidden = true;
				bullets[j].hidden = true;
				kills += 1;
			}
		}
	}
	
	/*collide, plane, enemy*/
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[i].hidden == true) continue;
		if (plane.hidden == true) break;
		if (collide(enemies[i], plane)) {
			enemies[i].hidden = true;
			plane.hp = 0;
		}
	}
	
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[i].hidden == true) continue;
		if (planf.hidden == true) break;
		if (collide(enemies[i], planf)) {
			enemies[i].hidden = true;
			planf.hp = 0;
		}
	}
}

static void draw_movable_object(MovableObject obj) {
    if (obj.hidden)
        return;
    al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
    if (draw_gizmos) {
        al_draw_rectangle(round(obj.x - obj.w / 2), round(obj.y - obj.h / 2),
            round(obj.x + obj.w / 2) + 1, round(obj.y + obj.h / 2) + 1, al_map_rgb(255, 0, 0), 0);
    }
}

static void draw(void) {
	
	if (plane.hidden == true && planf.hidden == true) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		char game_over_num[20] = "Game over";
		al_draw_text(font_72, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, game_over_num);
		
		al_draw_bitmap(right_background, SCREEN_W, 0, 0);
		char kills_num[20] = "KILLS: ";
		trans(kills_num + 7, kills);
		al_draw_text(font_60, al_map_rgb(0, 0, 0), SCREEN_W + 100, 200, 0, kills_num);
		
		return;
	}
	
	al_draw_bitmap(right_background, SCREEN_W, 0, 0);
    al_draw_bitmap(img_background, 0, 0, 0);
	for (int i = 0; i < MAX_BULLET; ++ i) {
		draw_movable_object(bullets[i]);
	}
    draw_movable_object(plane);
    draw_movable_object(planf);
    for (int i = 0; i < MAX_ENEMY; i++)
        draw_movable_object(enemies[i]);
        
    /*
	char plane_hp[20] = "HP: ";
    trans(plane_hp + 4, (int)plane.hp);
	al_draw_text(font_32, al_map_rgb(255, 255, 255), SCREEN_W - 250, SCREEN_H - 300, 0, plane_hp);
	*/
	char kills_num[20] = "KILLS: ";
	trans(kills_num + 7, kills);
	al_draw_text(font_48, al_map_rgb(0, 0, 0), SCREEN_W + 100, 200, 0, kills_num);
	/*
	char lives_num[20] = "LIVES: ";
	trans(lives_num + 7, lives);
	al_draw_text(font_32, al_map_rgb(255, 255, 255), SCREEN_W - 250, SCREEN_H - 500, 0, lives_num);
	
	if (plane.hidden == true) {
		char dying[25] = "Will respawn in   sec";
		dying[16] = 3 - (int)(al_get_time() - last_die) + '0';
		al_draw_text(font_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, dying);
	}
	*/
}

static void destroy(void) {
    al_destroy_bitmap(img_background);
    al_destroy_bitmap(img_plane);
    al_destroy_bitmap(img_enemy);
    al_destroy_sample(bgm);
    al_destroy_bitmap(img_bullet);
    stop_bgm(bgm_id);
    game_log("Story scene destroyed");
}

static void on_key_down(int keycode) {
    if (keycode == ALLEGRO_KEY_TAB)
        draw_gizmos = !draw_gizmos;
	if (keycode == ALLEGRO_KEY_BACKSPACE) {
		game_change_scene(scene_menu_create());
		al_rest(1.5);
	}
}

// TODO: Add more event callback functions such as keyboard, mouse, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_practice_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Start";
    scene.initialize = &init;
    scene.update = &update;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;
    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Start scene created");
    return scene;
}
