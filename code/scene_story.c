#include "scene_story.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "game.h"
#include "utility.h"
#include "shared.h"
#include "scene_menu.h"
#include <math.h>
#include <ctype.h>

#define pi acos(-1)

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static ALLEGRO_BITMAP* img_background;
static ALLEGRO_BITMAP* img_plane;
static ALLEGRO_BITMAP* img_bullet;
static ALLEGRO_BITMAP* right_background;

static void init(void);
static void update(void);
static void draw_movable_object(MovableObject obj);
static void draw(void);
static void destroy(void);
static void draw_draw_plane(MovableObject obj);

#define MAX_BULLET 40
static MovableObject plane;
static MovableObject bullets[MAX_BULLET];

static const float MAX_COOLDOWN = 0.2f;
static double last_shoot_timestamp;
static ALLEGRO_SAMPLE* bgm;
static ALLEGRO_SAMPLE_ID bgm_id;
static bool draw_gizmos;

static double last_enemy;
static int kills;
static int lives;
static double last_die;
static bool restart;
static double restart_time;

#define dBATTLE 0
#define dSTORY 1
static int mode;

char story_text[30][100] =  {
	"The Earth is being invaded by aliens",
	"They are attacking your hometown",
	"Manage to drive them back",
	"",
	"They come again, again, and again",
	"Almost everyone loses their home",
	"February in Tokyo",
	"",
	"June in Washington",
	"",
	"September in London",
	"",
	"November in Cairo",
	"",
	"Finally, aliens take over almost all the Earth",
	"Surrounding humans' last territory",
	"December in Taiwan",
	"It's the last war",
	"Fight to the end, pray for the miracle",
	"",
	"Everyone has gone all out",
	"It's your last gasp",
	"May humans' future bright"
};

static int text_id;
static int text_len;
static double last_story_time;
static double diff_story_time;
static ALLEGRO_SAMPLE* click;
static ALLEGRO_SAMPLE_ID click_id;
static int clicked;
static int skip;

#define MAX_ENEMY 50
#define MAX_ENEMY_BULLET 1000
static MovableObject enemies[10][MAX_ENEMY];
static MovableObject enemy_bullets[10][MAX_ENEMY_BULLET];
static int enemy_num[10];
static int enemy_wave;
static int enemy_score[10];
static int boss_hp[10];

static double now;
static bool paused;

static double skill_time;
static double skill_radius;
static int power;
static bool skill;
ALLEGRO_BITMAP *rocket;

static double ww, hh;

static int t_sz;
static int territory[6000];
static int we, he;
static int t_next;

static ALLEGRO_SAMPLE *bullet_s, *boss_s, *battle_s, *end_s;
static ALLEGRO_SAMPLE_ID bullet_id, boss_id, battle_id, end_id;

/* ////////////////////////
To do
boss
*//////////////////////////

bool clear() {
	for (int i = 0; i < 7; ++ i)
		for (int j = 0; j < MAX_ENEMY; ++ j)
			if (enemies[i][j].hidden == false)
				return false;
	for (int i = 0; i < 7; ++ i)
		if (enemy_num[i])
			return false;
	return true;
}

bool conquer() {
	int cnt = 0;
	for (int i = 0; i < t_sz; ++ i)
		cnt += (enemy_bullets[6][i].hidden == false);
	return t_sz / 2 <= cnt;
}

static void boss_init() {
	{
		enemies[3][0].img = load_bitmap_resized("./images/enemy/rocket-1.png", 150, 150);
		enemies[3][0].w = al_get_bitmap_width(enemies[3][0].img);
		enemies[3][0].h = al_get_bitmap_height(enemies[3][0].img);
		enemies[3][0].hidden = true;
		enemies[3][0].hp = boss_hp[3] = 1000;
		ALLEGRO_BITMAP* bullets_img = load_bitmap_resized("./images/bullets/bullet.bmp", 20, 20);
		for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
			enemy_bullets[3][i].img = bullets_img;
			enemy_bullets[3][i].w = al_get_bitmap_width(bullets_img);
			enemy_bullets[3][i].h = al_get_bitmap_height(bullets_img);
		}
	}
	{
		enemies[4][0].img = load_bitmap_resized("./images/enemy/rocket-2.png", 150, 150);
		enemies[4][0].w = al_get_bitmap_width(enemies[3][0].img);
		enemies[4][0].h = al_get_bitmap_height(enemies[3][0].img);
		enemies[4][0].hidden = true;
		enemies[4][0].hp = boss_hp[4] = 1800;
		ALLEGRO_BITMAP* bullets_img = load_bitmap_resized("./images/bullets/bullet.bmp", 20, 20);
		for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
			enemy_bullets[4][i].img = bullets_img;
			enemy_bullets[4][i].w = al_get_bitmap_width(bullets_img);
			enemy_bullets[4][i].h = al_get_bitmap_height(bullets_img);
		}
	}
	{
		enemies[5][0].img = load_bitmap_resized("./images/enemy/rocket-2.png", 150, 150);
		enemies[5][0].w = al_get_bitmap_width(enemies[3][0].img);
		enemies[5][0].h = al_get_bitmap_height(enemies[3][0].img);
		enemies[5][0].hidden = true;
		enemies[5][0].hp = boss_hp[5] = 2500;
		ALLEGRO_BITMAP* bullets_img = load_bitmap_resized("./images/bullets/bullet.bmp", 20, 20);
		for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
			enemy_bullets[5][i].img = bullets_img;
			enemy_bullets[5][i].w = al_get_bitmap_width(bullets_img);
			enemy_bullets[5][i].h = al_get_bitmap_height(bullets_img);
		}
	}
	{
		enemies[6][0].img = load_bitmap_resized("./images/enemy/rocket-2.png", 150, 150);
		enemies[6][0].w = al_get_bitmap_width(enemies[3][0].img);
		enemies[6][0].h = al_get_bitmap_height(enemies[3][0].img);
		enemies[6][0].hidden = true;
		enemies[6][0].hp = boss_hp[6] = 50000;
		ALLEGRO_BITMAP* bullets_img = load_bitmap_resized("./images/bullets/bullet.bmp", 50, 50);
		for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
			enemy_bullets[6][i].img = bullets_img;
			enemy_bullets[6][i].w = al_get_bitmap_width(bullets_img);
			enemy_bullets[6][i].h = al_get_bitmap_height(bullets_img);
		}
		
		t_sz = 0;
		we = he = 50;
		for (int i = we / 2; i < SCREEN_W; i += we)
			for (int j = he / 2; j < SCREEN_H; j += he)
				territory[t_sz ++] = i * 10000 + j;
		shuffle(territory, t_sz);
		for (int i = 0; i < t_sz; ++ i) {
			enemy_bullets[6][i].x = territory[i] / 10000;
			enemy_bullets[6][i].y = territory[i] % 10000;
		}
		t_next = 0;
	}
}

static void enemy_init() {
	paused = false;
	
	enemy_wave = 0;
	for (int i = 0; i < 7; ++ i) enemy_num[i] = 0;
	for (int i = 0; i < 7; ++ i)
		for (int j = 0; j < MAX_ENEMY; ++ j)
			enemies[i][j].hidden = true;
	for (int i = 0; i < 7; ++ i)
		for (int j = 0; j < MAX_ENEMY_BULLET; ++ j)
			enemy_bullets[i][j].hidden = true;
			
	enemy_score[0] = 20;
	enemy_score[1] = 3;
	enemy_score[2] = 50;
	enemy_score[3] = 150;
	
	ALLEGRO_BITMAP* img_1 = load_bitmap_resized("./images/enemy/ufo-1.png", 50, 50);
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		enemies[0][i].img = img_1;
		enemies[0][i].w = al_get_bitmap_width(img_1);
		enemies[0][i].h = al_get_bitmap_height(img_1);
		enemies[0][i].hidden = true;
	}
	
	ALLEGRO_BITMAP* img_2 = load_bitmap_resized("./images/enemy/ufo-2.png", 50, 50);
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		enemies[1][i].img = img_2;
		enemies[1][i].w = al_get_bitmap_width(img_2);
		enemies[1][i].h = al_get_bitmap_height(img_2);
		enemies[1][i].hidden = true;
	}
	
	ALLEGRO_BITMAP* img_3 = load_bitmap_resized("./images/enemy/ufo-3.png", 50, 50);
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		enemies[2][i].img = img_3;
		enemies[2][i].w = al_get_bitmap_width(img_3);
		enemies[2][i].h = al_get_bitmap_height(img_3);
		enemies[2][i].hidden = true;
	}
	
	ALLEGRO_BITMAP* bullets_img = load_bitmap_resized("./images/bullets/bullet.bmp", 20, 20);
	for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
		enemy_bullets[0][i].img = bullets_img;
		enemy_bullets[0][i].w = al_get_bitmap_width(bullets_img);
		enemy_bullets[0][i].h = al_get_bitmap_height(bullets_img);
		enemy_bullets[0][i].hidden = true;
	}
	
	for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
		enemy_bullets[1][i].img = bullets_img;
		enemy_bullets[1][i].w = al_get_bitmap_width(bullets_img);
		enemy_bullets[1][i].h = al_get_bitmap_height(bullets_img);
		enemy_bullets[1][i].hidden = true;
	}
	
	for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
		enemy_bullets[2][i].img = bullets_img;
		enemy_bullets[2][i].w = al_get_bitmap_width(bullets_img);
		enemy_bullets[2][i].h = al_get_bitmap_height(bullets_img);
		enemy_bullets[2][i].hidden = true;
	}
	boss_init();
}

static void enemy1_update() {
	static double last_enemy = -10;
	static double last_enemy_bullet = -10;
	
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[0][i].hidden == true && 2.0f <= now - last_enemy && enemy_num[0] > 0) {
			enemies[0][i].x = random(enemies[0][i].w / 2, SCREEN_W - enemies[0][i].w / 2);
			enemies[0][i].y = -enemies[0][i].h / 2;
			enemies[0][i].vx = random(100, 300);
			enemies[0][i].vy = 7.5;
			enemies[0][i].hidden = false;
			enemies[0][i].hp = 60;
			last_enemy = now;
			enemy_num[0] -= 1;
			break;
		}
	}
	
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[0][i].hidden == true) 
			continue;
		enemies[0][i].y += enemies[0][i].vy;
		if (enemies[0][i].vx - 10 <= enemies[0][i].y)
			enemies[0][i].vy = 7.5 * (enemies[0][i].vx - enemies[0][i].y) / 10;
	}
	
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[0][i].hidden == false && enemies[0][i].vx - enemies[0][i].y <= 0.1 && 1 <= now - last_enemy_bullet) {
			for (int j = 0; j < MAX_ENEMY_BULLET; ++ j) {
				if (enemy_bullets[0][j].hidden == true) {
					enemy_bullets[0][j].x = enemies[0][i].x;
					enemy_bullets[0][j].y = enemies[0][i].y + enemies[0][i].h / 2;
					enemy_bullets[0][j].vx = random(-3, 3);
					enemy_bullets[0][j].vy = 3;
					enemy_bullets[0][j].hidden = false;
					break;
				}
			}
		}
	}
	
	if (1 <= now - last_enemy_bullet) {
		last_enemy_bullet = now;
	}
	
	for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
		if (enemy_bullets[0][i].hidden == false) {
			enemy_bullets[0][i].x += enemy_bullets[0][i].vx;
			enemy_bullets[0][i].y += enemy_bullets[0][i].vy;
			if (cross(enemy_bullets[0][i]) == 0) {
				enemy_bullets[0][i].hidden = true;
			}
		}
	}
}

static void enemy2_update() {
	static double last_enemy = -10;
	
	static int cnt = 0;
	static MovableObject pre;
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[1][i].hidden == true && 2.0f <= now - last_enemy && cnt == 0 && enemy_num[1] > 0) {
			enemies[1][i].x = random(enemies[0][i].w / 2 + 150, SCREEN_W - enemies[0][i].w / 2 - 150);
			enemies[1][i].y = -enemies[0][i].h / 2;
			enemies[1][i].vx = random(2, 4);
			enemies[1][i].vy = 2;
			enemies[1][i].hidden = false;
			enemies[1][i].hp = 1;
			last_enemy = now;
			cnt += 1, pre = enemies[1][i];
			enemy_num[1] -= 1;
			break;
		}
	}
	
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[1][i].hidden == true && 0.4f <= now - last_enemy && cnt != 0 && enemy_num[1] > 0) {
			enemies[1][i] = pre;
			cnt = (cnt + 1) % 3;
			last_enemy = now;
			enemy_num[1] -= 1;
			break;
		}
	}
	
	for (int i = 0; i < MAX_ENEMY; ++ i) {
		if (enemies[1][i].hidden == false) {
			enemies[1][i].x += enemies[1][i].vx * cos(enemies[1][i].y / 100);
			enemies[1][i].y += enemies[1][i].vy;
			if (enemies[1][i].x < plane.w / 2) 
				enemies[1][i].x = plane.w / 2;
			if (SCREEN_W - plane.w / 2 < enemies[1][i].x)
				enemies[1][i].x = SCREEN_W - plane.w / 2;
			if (SCREEN_H + enemies[1][i].h / 2 < enemies[1][i].y)
				enemies[1][i].hidden = true;
		} 
	}
}

static void enemy3_update() {
	if (enemy_num[2] > 0 && enemies[2][0].hidden == true && enemies[2][1].hidden == true) {
		enemies[2][0].hidden = false;
		enemies[2][0].x = -enemies[2][0].w / 2;
		enemies[2][0].y = random(400, SCREEN_H - 400);
		enemies[2][0].vx = 3;
		enemies[2][0].vy = random(100, 500);
		enemies[2][0].hp = 200;
		
		enemies[2][1] = enemies[2][0];
		enemies[2][1].x = SCREEN_W + enemies[2][1].w / 2;
		enemies[2][1].vx *= -1;
		enemies[2][1].vy = random(SCREEN_W - 500, SCREEN_W - 100);
		enemy_num[2] -= 2;
		if (enemy_num[2] < 0) enemy_num[2] = 0;
	}
	else if (enemy_num[2] > 0 && enemies[2][1].hidden == true && enemies[2][0].hidden == false) {
		enemies[2][1] = enemies[2][0];
		enemies[2][1].x = SCREEN_W + enemies[2][1].w / 2;
		enemies[2][1].vx *= -1;
		enemies[2][1].vy = random(SCREEN_W - 500, SCREEN_W - 100);
		enemy_num[2] -= 1;
	}
	else if (enemy_num[2] > 0 && enemies[2][0].hidden == true && enemies[2][1].hidden == false) {
		enemies[2][0] = enemies[2][1];
		enemies[2][0].x = -enemies[2][0].w / 2;
		enemies[2][0].vx *= -1;
		enemies[2][0].vy = random(100, 500);
		enemy_num[2] -= 1;
	}
	
	
	for (int i = 0; i < 2; ++ i) {
		if (enemies[2][i].hidden == false) {
			if (abs(enemies[2][i].x - enemies[2][i].vy) >= 10) 
				enemies[2][i].x += enemies[2][i].vx;
		}
	}
	
	static double last_shot = -10;
	
	for (int i = 0; i < 2; ++ i) {
		if (enemies[2][i].hidden == false && abs(enemies[2][i].x - enemies[2][i].vy) <= 10 && 1.5f <= now - last_shot) {
			int cnt = 3;
			double angle = random(0, 2 * acos(-1));
			for (int j = 0; j < MAX_ENEMY_BULLET && 0 < cnt; ++ j) {
				if (enemy_bullets[2][j].hidden == true) {
					enemy_bullets[2][j].x = enemies[2][i].x;
					enemy_bullets[2][j].y = enemies[2][i].y;
					enemy_bullets[2][j].vx = 3 * cos(angle + cnt * (2 * acos(-1) / 3));
					enemy_bullets[2][j].vy = 3 * sin(angle + cnt * (2 * acos(-1) / 3));
					enemy_bullets[2][j].hidden = false;
					cnt -= 1;
				}
			}
		}
	}
	if (1.5f <= now - last_shot) {
		last_shot = now;
	}
	
	for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
		if (enemy_bullets[2][i].hidden == false) {
			enemy_bullets[2][i].x += enemy_bullets[2][i].vx;
			enemy_bullets[2][i].y += enemy_bullets[2][i].vy;
			if (cross(enemy_bullets[2][i]) == 0) {
				enemy_bullets[2][i].hidden = true;
			}
		}
	}
}

static void boss_update() {
	{
		if (enemy_num[3] != 0) {
			enemies[3][0].hidden = false;
			enemies[3][0].x = SCREEN_W / 2;
			enemies[3][0].y = -enemies[3][0].h / 2;
			enemies[3][0].vy = 10;
			enemy_num[3] -= 1;
		}
		static double last_shot_boss1 = -10;
		if (enemies[3][0].hidden == false) {
			if (enemies[3][0].y < 300)
				enemies[3][0].y += enemies[3][0].vy;
			else if (1.5f <= now - last_shot_boss1) {
				last_shot_boss1 = now;
				double angle = random(0, 360.0);
				double vel = random(2, 4.5);
				int cnt = 60;
				for (int i = 0; i < MAX_ENEMY_BULLET && cnt; ++ i) {
					if (enemy_bullets[3][i].hidden == false) 
						continue;
					enemy_bullets[3][i].x = enemies[3][0].x;
					enemy_bullets[3][i].y = enemies[3][0].y;
					enemy_bullets[3][i].vx = vel * cos(angle + cnt * (2 * pi / 60.0));
					enemy_bullets[3][i].vy = vel * sin(angle + cnt * (2 * pi / 60.0));
					enemy_bullets[3][i].hidden = false;
					cnt -= 1;
				}
			}
		}
		for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
			if (enemy_bullets[3][i].hidden == false) {
				enemy_bullets[3][i].x += enemy_bullets[3][i].vx;
				enemy_bullets[3][i].y += enemy_bullets[3][i].vy;
				if (cross(enemy_bullets[3][i]) == 0)
					enemy_bullets[3][i].hidden = true;
			}
		}
	}
	
	{
		if (enemy_num[4] != 0) {
			enemies[4][0].hidden = false;
			enemies[4][0].x = SCREEN_W + enemies[4][0].w / 2;
			enemies[4][0].y = 300;
			enemies[4][0].vx = 10;
			enemy_num[4] -= 1;
		}
		static double last_shot_boss2 = -10;
		if (enemies[4][0].hidden == false) {
			if (SCREEN_W / 2 < enemies[4][0].x)
				enemies[4][0].x -= enemies[4][0].vx;
			else if (0.1f <= now - last_shot_boss2) {
				last_shot_boss2 = now;
				static double angle = 0;
				double vel = 3;
				int cnt = 6;
				for (int i = 0; i < MAX_ENEMY_BULLET && cnt; ++ i) {
					if (enemy_bullets[4][i].hidden == false) 
						continue;
					enemy_bullets[4][i].x = enemies[4][0].x;
					enemy_bullets[4][i].y = enemies[4][0].y;
					enemy_bullets[4][i].vx = vel * cos(angle + cnt * (2 * pi / 6.0));
					enemy_bullets[4][i].vy = vel * sin(angle + cnt * (2 * pi / 6.0));
					enemy_bullets[4][i].hidden = false;
					cnt -= 1;
				}
				angle += 0.1;
			}
		}
		for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
			if (enemy_bullets[4][i].hidden == false) {
				enemy_bullets[4][i].x += enemy_bullets[4][i].vx;
				enemy_bullets[4][i].y += enemy_bullets[4][i].vy;
				if (cross(enemy_bullets[4][i]) == 0)
					enemy_bullets[4][i].hidden = true;
			}
		}
	}
	
	{
		if (enemy_num[5] != 0) {
			enemies[5][0].hidden = false;
			enemies[5][0].x = - enemies[5][0].w / 2;
			enemies[5][0].y = 300;
			enemies[5][0].vx = 10;
			enemy_num[5] -= 1;
		}
		static double last_shot_boss3 = -10;
		static double last_hello_time = -10;
		static int cnt = 0;
		
		double ww = enemy_bullets[5][0].w * 2;
		double hh = enemy_bullets[5][0].h * 2;
		if (enemies[5][0].hidden == false) {
			
			if (enemies[5][0].x < SCREEN_W / 2)
				enemies[5][0].x += enemies[5][0].vx;
			else if (0.005f <= now - last_shot_boss3 && 6.5 <= now - last_hello_time) {
				last_shot_boss3 = now;
				for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
					if (enemy_bullets[5][i].hidden == false) 
						continue;
					if (cnt < SCREEN_W / ww) {
						enemy_bullets[5][i].x = cnt * ww + ww / 2;
						enemy_bullets[5][i].y = hh / 2;
					}
					else if (cnt < SCREEN_W / ww + SCREEN_H / hh) {
						enemy_bullets[5][i].x = SCREEN_W - ww / 2;
						enemy_bullets[5][i].y = (cnt - SCREEN_W / ww) * hh + hh / 2;
					}
					else if (cnt < SCREEN_W * 2 / ww + SCREEN_H / hh) {
						enemy_bullets[5][i].x = SCREEN_W - ww / 2 - (cnt - SCREEN_W / ww - SCREEN_H / hh) * ww;
						enemy_bullets[5][i].y = SCREEN_H - hh / 2;
					}
					else if (cnt < SCREEN_W * 2 / ww + SCREEN_H * 2 / hh) {
						enemy_bullets[5][i].x = ww / 2;
						enemy_bullets[5][i].y = SCREEN_H - hh / 2 - (cnt - SCREEN_W * 2 / ww - SCREEN_H / hh) * hh;
					}
					enemy_bullets[5][i].vx = 0;
					enemy_bullets[5][i].vy = 0;
					enemy_bullets[5][i].hidden = false;
					
					cnt += 1;
					if (cnt == SCREEN_W * 2 / ww + SCREEN_H * 2 / hh) {
						last_hello_time = now;
						for (int j = 0; j < MAX_ENEMY_BULLET; ++ j) {
							if (enemy_bullets[5][j].hidden == true) continue;
							
							if (enemy_bullets[5][j].y == hh / 2)
								enemy_bullets[5][j].vy = random(1, 3);
							else if (enemy_bullets[5][j].x == SCREEN_W - ww / 2)
								enemy_bullets[5][j].vx = -random(1, 3);
							else if (enemy_bullets[5][j].y == SCREEN_H - hh / 2)
								enemy_bullets[5][j].vy = -random(1, 3);
							else if (enemy_bullets[5][j].x == ww / 2) {
								enemy_bullets[5][j].vx = random(1, 3);
							}
								
						}
						cnt = 0;
					}
					break;
				}
			}
		}
		if (enemies[5][0].hp <= 0) {
			for (int j = 0; j < MAX_ENEMY_BULLET; ++ j) {
				if (enemy_bullets[5][j].hidden == true) continue;
				
				if (enemy_bullets[5][j].y == hh / 2)
					enemy_bullets[5][j].vy = random(1, 3);
				else if (enemy_bullets[5][j].x == SCREEN_W - ww / 2)
					enemy_bullets[5][j].vx = -random(1, 3);
				else if (enemy_bullets[5][j].y == SCREEN_H - hh / 2)
					enemy_bullets[5][j].vy = -random(1, 3);
				else if (enemy_bullets[5][j].x == ww / 2) {
					enemy_bullets[5][j].vx = random(1, 3);
				}
					
			}
		}
		for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
			if (enemy_bullets[5][i].hidden == false) {
				enemy_bullets[5][i].x += enemy_bullets[5][i].vx;
				enemy_bullets[5][i].y += enemy_bullets[5][i].vy;
				if (cross(enemy_bullets[5][i]) == 0)
					enemy_bullets[5][i].hidden = true;
			}
		}
	}

	{
		static double last_shot_boss4 = -10;
		if (enemy_num[6] != 0) {
			last_shot_boss4 = now;
			enemies[6][0].img = load_bitmap_resized("./images/enemy/rocket-2.png", 0, 0);
			enemies[6][0].hidden = false;
			enemies[6][0].x = SCREEN_W / 2;
			enemies[6][0].y = 300;
			enemy_num[6] -= 1;
		}

		
		static double shot_time_gap = 5;
		static double last_bigger = -10;
		static double sw = 0, sh = 0;
		
		if (enemies[6][0].hidden == false) {
			if (0.001f <= now - last_bigger && sw < 200) {
				last_bigger = now;
				sw += 0.2;
				sh += 0.2;
				enemies[6][0].img = load_bitmap_resized("./images/enemy/rocket-2.png", sw, sh);
				enemies[6][0].w = sw;
				enemies[6][0].h = sh;
			}
			if (shot_time_gap <= now - last_shot_boss4) {
				last_shot_boss4 = now;
				for (int i = 0; i < t_sz; ++ i) {
					if (enemy_bullets[6][i].hidden == false) 
						continue;
					shot_time_gap -= 0.1;
					enemy_bullets[6][i].hidden = false;
					break;
				}
				for (int i = 0; i < MAX_ENEMY_BULLET; ++ i) {
					if (enemy_bullets[6][i].hidden == true) {
						t_next = i;
						break;
					}
				}
			}
		}
	}
}

static void enemy_update() {
	static double pre_wave;
	
	if (enemy_wave % 2 == 0 && clear()) {
		plane.hidden = false;
	}
	
	if (enemy_wave == 0 && clear()) {
		enemy_wave += 1;
		enemy_num[0] = 3;
		enemy_num[1] = 15;
		enemy_num[2] = 2;
		pre_wave = al_get_time();
	}
	if (enemy_wave == 1) {
		if (now - pre_wave >= 1.0f) enemy2_update();
		if (now - pre_wave >= 5.0f) enemy1_update();
		if (now - pre_wave >= 12.0f) enemy3_update();
		if (now - pre_wave >= 1.0f) boss_update();
	}
	if (enemy_wave == 2 && clear()) {
		stop_bgm(bgm_id);
		battle_id = play_bgm(battle_s, 1);
		enemy_wave += 1;
		enemy_num[0] = 8;
		enemy_num[1] = 42;
		enemy_num[2] = 3;
		pre_wave = al_get_time();
	}
	if (enemy_wave == 3) {
		if (now - pre_wave >= 1.0f) enemy2_update();
		if (now - pre_wave >= 5.0f) enemy1_update();
		if (now - pre_wave >= 10.0f) enemy3_update();
		if (now - pre_wave >= 1.0f) boss_update();
	}
	if (enemy_wave == 4 && clear()) {
		enemy_wave += 1;
		enemy_num[0] = 10;
		enemy_num[1] = 54;
		enemy_num[2] = 4;
		enemy_num[3] = 1;
		pre_wave = al_get_time();
	}
	if (enemy_wave == 5) {
		if (now - pre_wave >= 1.0f) enemy2_update();
		if (now - pre_wave >= 5.0f) enemy1_update();
		if (now - pre_wave >= 10.0f) enemy3_update();
		if (now - pre_wave >= 15.0f) boss_update();
	}
	if (enemy_wave == 6 && clear()) {
		enemy_wave += 1;
		enemy_num[0] = 15;
		enemy_num[1] = 78;
		enemy_num[2] = 5;
		enemy_num[4] = 1;
		pre_wave = al_get_time();
	}
	if (enemy_wave == 7) {
		if (now - pre_wave >= 1.0f) enemy2_update();
		if (now - pre_wave >= 4.0f) enemy1_update();
		if (now - pre_wave >= 8.0f) enemy3_update();
		if (now - pre_wave >= 14.0f) boss_update();
	}
	if (enemy_wave == 8 && clear()) {
		enemy_wave += 1;
		enemy_num[0] = 15;
		enemy_num[1] = 78;
		enemy_num[2] = 6;
		enemy_num[5] = 1;
		pre_wave = al_get_time();
	}
	if (enemy_wave == 9) {
		if (now - pre_wave >= 1.0f) enemy2_update();
		if (now - pre_wave >= 3.0f) enemy1_update();
		if (now - pre_wave >= 6.0f) enemy3_update();
		if (now - pre_wave >= 11.0f) boss_update();
	}
	if (enemy_wave == 10 && clear()) {
		stop_bgm(battle_id);
		boss_id = play_bgm(boss_s, 1);
		enemy_wave += 1;
		enemy_num[0] = 25;
		enemy_num[1] = 300;
		enemy_num[2] = 10;
		enemy_num[6] = 1;
		pre_wave = al_get_time();
	}
	if (enemy_wave == 11) {
		if (now - pre_wave >= 1.0f) enemy2_update();
		if (now - pre_wave >= 5.0f) enemy1_update();
		if (now - pre_wave >= 12.0f) enemy3_update();
		if (now - pre_wave >= 0.0f) boss_update();
	}
}

static void init(void) {
	al_hide_mouse_cursor(game_display);
	{
		bullet_s = load_audio("./bgm/bullet.ogg");
	}
	
	mode = dSTORY;
	{
		text_id = 0;
		text_len = 0;
		last_story_time = al_get_time();
		click = load_audio("./bgm/text.ogg");
		clicked = 0;
		skip = 0;
		diff_story_time = 0;
	}
	
	srand(time(NULL));
	{
		kills = 0;
		lives = 5;
		last_die = -10;
		restart = false;
		power = 5;
		skill_time = -10;
		skill_radius = 0;
	}
	
	right_background = load_bitmap_resized("./images/background/setting.bmp", WINDOW_W - SCREEN_W, WINDOW_H);
    img_background = load_bitmap_resized("./images/background/chapter_one.bmp", SCREEN_W, SCREEN_H);
	
   	if (staff_name == 0) img_plane = plane.img = load_bitmap_resized("./images/staff/plane.png", 75, 75);
   	else img_plane = plane.img = load_bitmap_resized("./images/staff/tank-2.png", 75, 75);
    ww = al_get_bitmap_width(plane.img);
    hh = al_get_bitmap_height(plane.img);
    plane.w = plane.h = 10;
    plane.x = SCREEN_W / 2;
    plane.y = SCREEN_H - hh * 3;
    plane.vx = plane.vy = 3;
    plane.hp = 100;
    
    rocket = al_load_bitmap("./images/bullets/rocket.png");
    img_bullet = al_load_bitmap("./images/bullets/bullet_1.png");
    for (int i = 0; i < MAX_BULLET; ++ i) {
		bullets[i].img = img_bullet;
		bullets[i].w = al_get_bitmap_width(img_bullet);
		bullets[i].h = al_get_bitmap_height(img_bullet);
		bullets[i].vx = 0;
		bullets[i].vy = -30;
		bullets[i].hidden = true;
		bullets[i].atk = 20;
	}
    enemy_init();
    
    // Can be moved to shared_init to decrease loading time.
    bgm = load_audio("./bgm/fight.ogg");
    game_log("Start scene initialized");
    bgm_id = play_bgm(bgm, 1);
    
    battle_s = load_audio("./bgm/battle.ogg");
    boss_s = load_audio("./bgm/boss.ogg");
    end_s = load_audio("./bgm/end.ogg");
}

static void story_update(void) {
	double now = al_get_time();
	
	diff_story_time = 0;
	if (skip == 1) {
		diff_story_time = now - last_story_time;
		if (diff_story_time >= 2.0f) {
			text_len = 0;
			while (strlen(story_text[text_id]) != 0)
				text_id += 1;
	
			mode = dBATTLE;
			plane.x = SCREEN_W / 2;
   			plane.y = SCREEN_H - hh * 3;
			al_set_mouse_xy(game_display, plane.x, plane.y);
			mouse_x = plane.x;
			mouse_y = plane.y;
			skip = 0;
			
			if (enemy_wave == 2) img_background = load_bitmap("./images/background/tokyo.png");
			if (enemy_wave == 4) img_background = load_bitmap("./images/background/washington.png");
			if (enemy_wave == 6) img_background = load_bitmap("./images/background/london.png");
			if (enemy_wave == 8) img_background = load_bitmap("./images/background/cairo.png");
			if (enemy_wave == 10) img_background = load_bitmap("./images/background/taiwan.png");
		}
		return;
	}
	
	if (text_len == 0) {
		if (1.0f <= now - last_story_time) {
			text_len += 1;
			clicked = 1;
			last_story_time = now;
			if (strlen(story_text[text_id]) == 0) {
				mode = dBATTLE;
				plane.x = SCREEN_W / 2;
    			plane.y = SCREEN_H - hh * 3;
				al_set_mouse_xy(game_display, plane.x, plane.y);
				mouse_x = plane.x;
				mouse_y = plane.y;
				
				if (enemy_wave == 2) img_background = load_bitmap("./images/background/tokyo.png");
				if (enemy_wave == 4) img_background = load_bitmap("./images/background/washington.png");
				if (enemy_wave == 6) img_background = load_bitmap("./images/background/london.png");
				if (enemy_wave == 8) img_background = load_bitmap("./images/background/cairo.png");
				if (enemy_wave == 10) img_background = load_bitmap("./images/background/taiwan.png");
			}
		}
		return;
	}
	if (0 < text_len && text_len < strlen(story_text[text_id])) {
		if (0.2f <= now - last_story_time) {
			text_len += 1;
			last_story_time = now;
			if (isalpha(story_text[text_id][text_len - 1]))
				clicked = 1;
			return;
		}
	}
	if (text_len == strlen(story_text[text_id])) {
		diff_story_time = now - last_story_time;
		if (2.0f <= now - last_story_time) {
			text_len = 0;
			text_id += 1;
			last_story_time = now;
			diff_story_time = 0;
		}
	}
}

static void story_draw(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));
	char show_story[100];
	for (int i = 0; i < text_len; ++ i) show_story[i] = story_text[text_id][i];
	show_story[text_len] = '\0';
	int color = 255 - (diff_story_time >= 1) * (diff_story_time - 1) * 255;
	if (skip == 1) color = 255 - (diff_story_time <= 1) * diff_story_time * 255 - (diff_story_time >= 1) * 255;
	al_draw_text(font_60, al_map_rgb(color, color, color), WINDOW_W / 2, WINDOW_H / 2 - 100, ALLEGRO_ALIGN_CENTER, show_story);
	if (clicked == 1) {
		click_id = play_audio(click, 1);
		clicked = 0;
		al_rest(0.2);
	}
}

static void story_on_key_down(int keycode) {
	if (keycode == ALLEGRO_KEY_ENTER) {
		skip = 1;
	}
	if (keycode == ALLEGRO_KEY_BACKSPACE) {
		game_change_scene(scene_menu_create());
		al_rest(1.5);
	}
}

static void battle_update(void) {
	if (enemy_wave == 12) return;
	
	if (paused) return;
	if (lives < 0) return;
	
	if (skill_radius != 0) {
		skill_radius += 20;
		for (int i = 0; i < 7; ++ i)
			for (int j = 0; j < MAX_ENEMY_BULLET; ++ j)
				if (dist(enemy_bullets[i][j], plane) <= skill_radius)
					enemy_bullets[i][j].hidden = true;
		if (skill_radius >= 400)
			skill_radius = 0;
	}
	
	if (now < skill_time) {
		for (int i = 0; i < MAX_BULLET; ++ i) {
			if (bullets[i].hidden == false) continue;
			bullets[i].img = rocket;
			bullets[i].w = al_get_bitmap_width(rocket);
			bullets[i].h = al_get_bitmap_height(rocket);
			bullets[i].atk = 50;
		}
	}
	if (now > skill_time) {
		for (int i = 0; i < MAX_BULLET; ++ i) {
			if (bullets[i].atk == 20 || bullets[i].hidden == false) continue;
			bullets[i].img = img_bullet;
			bullets[i].w = al_get_bitmap_width(img_bullet);
			bullets[i].h = al_get_bitmap_height(img_bullet);
			bullets[i].atk = 20;
		}
		skill = false;
	}
	
	if (enemy_wave == 11 && clear()) {
		for (int i = 0; i < 7; ++ i)
			for (int j = 0; j < MAX_ENEMY; ++ j)
				enemies[i][j].hidden = true;
		for (int i = 0; i < 3; ++ i)
			for (int j = 0; j < MAX_BULLET; ++ j) {
				enemy_bullets[i][j].x += enemy_bullets[i][j].vx;
				enemy_bullets[i][j].y += enemy_bullets[i][j].vy;
				if (cross(enemy_bullets[i][j]) == 0)
					enemy_bullets[i][j].hidden = true;
			}
		for (int i = 0; i < MAX_BULLET; ++ i) {
			bullets[i].x += bullets[i].vx;
			bullets[i].y += bullets[i].vy;
			if (cross(bullets[i]) == 0)
				bullets[i].hidden = true;
		}
		return;
	}
	
	now = al_get_time();
	
	/*game over*/
	if (plane.hp <= 0) {
		if (plane.hidden == false) {
			plane.hidden = true;
			last_die = now;
			lives -= 1;
			if (lives < 0) {
				if (enemy_wave != 12)
					history_score[h_sz ++] = kills;
			}
		}
		else if (now - last_die >= 0) {
			plane.hidden = false;
			plane.hp = 100;
			plane.x = (int)SCREEN_W / 2 - (int)plane.w / 2;
		    plane.y = (int)SCREEN_H - (int)hh * 3;
			al_set_mouse_xy(game_display, plane.x, plane.y);
		}
	}
		
	if (enemy_wave < 11 && enemy_wave % 2 == 1 && clear()) {
		for (int i = 0; i < MAX_BULLET; ++ i) bullets[i].hidden = true;
		for (int i = 0; i < 7; ++ i)
			for (int j = 0; j < MAX_ENEMY; ++ j)
				enemies[i][j].hidden = true;
		for (int i = 0; i < 7; ++ i)
			for (int j = 0; j < MAX_ENEMY_BULLET; ++ j)
				enemy_bullets[i][j].hidden = true;
		plane.hidden = true;
		
		mode = dSTORY;
		text_id += 1;
		skip = 0;
		enemy_wave += 1;
		return;
	}
	
	/*plane*/ 
	plane.x = mouse_x;
	plane.y = mouse_y;
	
	if (plane.x < plane.w / 2)
		plane.x = plane.w / 2;
	else if (plane.x + plane.w / 2 > SCREEN_W)
		plane.x = SCREEN_W - plane.w / 2;
	if (plane.y < plane.h / 2)
		plane.y = plane.h / 2;
	else if (plane.y + plane.h / 2 > SCREEN_H)
		plane.y = SCREEN_H - plane.h / 2;
    
	/*bullet*/
    for (int i = 0; i < MAX_BULLET; ++ i) {
		if (bullets[i].hidden == true)
			continue;
		bullets[i].x += bullets[i].vx;
		bullets[i].y += bullets[i].vy;
		if (bullets[i].y < 0)
			bullets[i].hidden = true;
	}

	if (key_state[ALLEGRO_KEY_SPACE] && plane.hidden == false && now - last_shoot_timestamp >= MAX_COOLDOWN) {
		for (int i = 0; i < MAX_BULLET; ++ i) {
			if (bullets[i].hidden == true) {
				bullet_id = play_audio(bullet_s, 0.5);
				last_shoot_timestamp = now;
				bullets[i].hidden = false;
				bullets[i].x = plane.x;
				bullets[i].y = plane.y;
				break;
			}
		}
	}
	/*enemy*/
	enemy_update();
	
	/*collide, bullet, enemy*/ 
	for (int i = 0; i < MAX_BULLET; ++ i) {
		for (int j = 0; j < 7; ++ j) {
			if (bullets[i].hidden == true)
				break;
			for (int k = 0; k < MAX_ENEMY; ++ k) {
				if (enemies[j][k].hidden == false && collide(bullets[i], enemies[j][k])) {
					enemies[j][k].hp -= bullets[i].atk;
					bullets[i].hidden = true;
					if (enemies[j][k].hp <= 0) {
						enemies[j][k].hidden = true;	
						kills += enemy_score[j];
					}
					break;
				}
			}
		}
	}
	
	/*collide, plane, enemy*/
	
	for (int i = 0; i < 7; ++ i) {
		for (int j = 0; j < MAX_ENEMY; ++ j) {
			if (enemies[i][j].hidden == false && plane.hidden == false && collide(enemies[i][j], plane) && 3.0 <= now - last_die) {
				plane.hp = 0;
				break;
			}
		}
		for (int j = 0; j < MAX_ENEMY_BULLET; ++ j) {
			if (enemy_bullets[i][j].hidden == false && plane.hidden == false && collide(enemy_bullets[i][j], plane) && 3.0 <= now - last_die) {
				plane.hp = 0;
				break;
			}
		}
	}
	
	/*restart*/
	if (restart == true) {
		if (al_get_time() - restart_time >= 5) {
			game_change_scene(scene_story_create());
		}
	}
}

static void battle_draw(void) {
	static double bye_time = -10;
	if (enemy_wave == 12) {
		now = al_get_time();
		if (bye_time <= -10) {
			bye_time = now;
			if (h_sz != 0 && history_score[h_sz - 1] != kills)
				history_score[h_sz ++] = kills;
			stop_bgm(boss_id);
			end_id = play_bgm(end_s, 1);
			ddd = 1;
		}
		int hhh = SCREEN_H - 100 * (now - bye_time);
		char author[30] = "author:  PolarisChiba";
		char t[30] = "Specially thanks to IP1";
		char thx[30] = "Thank you for your playing";
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font_60, al_map_rgb(255, 255, 255), SCREEN_W / 2, hhh, ALLEGRO_ALIGN_CENTER, author);
		al_draw_text(font_60, al_map_rgb(255, 255, 255), SCREEN_W / 2, hhh + 100, ALLEGRO_ALIGN_CENTER, t);
		al_draw_text(font_60, al_map_rgb(255, 255, 255), SCREEN_W / 2, hhh + 800, ALLEGRO_ALIGN_CENTER, thx);
		
		if (hhh + 800 <= -200) {
			char wish[25] = "May humans\' future bright";
			static int color = 0;
			if (color < 255) color += 1;
			al_draw_text(font_72, al_map_rgb(color, color, color), SCREEN_W / 2, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_CENTER, wish);
		}
		
		
	}
	else if (enemy_wave == 11 && conquer() && lives < 0) {
		
		double clear_time = al_get_time();
		/*
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
		if (1 <= clear_time - now) al_clear_to_color(al_map_rgb(0, 0, 0));
		
		double transparent = 1;
		if (1 <= clear_time - now && clear_time - now <= 4) transparent = 1 - (clear_time - now - 1) / 3;
		else if (4 < clear_time - now) transparent = 0;
		al_draw_tinted_bitmap(img_background, al_map_rgba_f(1, 1, 1, transparent), 0, 0, 0);
	
		char clear_text[30] = "May human\'s future bright";
		double color = 0;
		if (4 <= clear_time - now && clear_time - now <= 10) color = 255 * (clear_time - now - 4) / 6;
		else if (10 < clear_time - now) color = 255;
		if (4 <= clear_time - now)
			al_draw_text(font_72, al_map_rgb(color, color, color), SCREEN_W / 2, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_CENTER, clear_text);
		*/
		if (clear_time - now) {
			mode = dSTORY;
			text_id += 1;
			skip = 0;
			enemy_wave += 1;
		}
	}
	else if (restart == true) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		char restart_num[20] = "Restart in   sec";
		restart_num[11] = 5 - (int)(al_get_time() - restart_time) + '0';
		al_draw_text(font_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, restart_num);
	}
	else if (lives < 0) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		char game_over_num[20] = "Game over";
		al_draw_text(font_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, game_over_num);
	}
	else {
		al_draw_bitmap(img_background, 0, 0, 0);
		for (int i = 0; i < MAX_BULLET; ++ i) {
			draw_movable_object(bullets[i]);
		}
	    draw_draw_plane(plane);
	    for (int i = 0; i < 7; ++ i)
	    	for (int j = 0; j < MAX_ENEMY; ++ j)
	    		draw_movable_object(enemies[i][j]);
		for (int i = 0; i < 7; ++ i)
	    	for (int j = 0; j < MAX_ENEMY_BULLET; ++ j)
	    		draw_movable_object(enemy_bullets[i][j]);
	    if (0.1 <= now - last_die && now - last_die <= 3 && clear() == false) {
	    	al_draw_circle(plane.x, plane.y, 50, al_map_rgb(0, 0, 0), 1);
		}
		if (skill_radius != 0) {
			al_draw_circle(plane.x, plane.y, skill_radius, al_map_rgb(0, 255, 0), 3);
		}
		
		for (int i = 3; i < 7; ++ i) {
			if (enemies[i][0].hidden == true) 
				continue;
			al_draw_filled_rectangle(50, 50, 50 + (SCREEN_W - 100) * enemies[i][0].hp / boss_hp[i], 60, al_map_rgb(200, 100, 0));
		}
		if (enemies[6][0].hidden == false && t_next != t_sz) {
			int x = territory[t_next] / 10000;
			int y = territory[t_next] % 10000;
			al_draw_filled_rectangle(x - we / 2, y - he / 2, x + we / 2, y + he / 2, al_map_rgb(100, 100, 100));
		}
	}
	//al_draw_bitmap(right_background, SCREEN_W, 0, 0);
	double ttrans = 0;//1 - 1.0 / 12.0 * enemy_wave;
	al_draw_bitmap(right_background, SCREEN_W, 0, 0);
	al_draw_filled_rectangle(SCREEN_W, 0, WINDOW_W, WINDOW_H, al_map_rgba_f(1.0 / 24.0 * enemy_wave, 0, 0, ttrans));
	//al_draw_tinted_bitmap(right_background, al_map_rgba_f(1, 1, 0, ttrans), SCREEN_W, 0, 0);
	char kills_num[20] = "SCORE:  ";
	trans(kills_num + 8, kills);
	al_draw_text(font_48, al_map_rgb(255, 255, 255), WINDOW_W - 550, 100, 0, kills_num);
	char lives_num[20] = "LIVES:  ";
	if (lives >= 0) trans(lives_num + 8, lives);
	al_draw_text(font_48, al_map_rgb(255, 255, 255), WINDOW_W - 550, 200, 0, lives_num);
	char power_num[20] = "POWER:  ";
	trans(power_num + 8, power);
	al_draw_text(font_48, al_map_rgb(255, 255, 255), WINDOW_W - 550, 300, 0, power_num);
	if (skill == true) {
		char skill_num[20] = "SKILL:  ";
		trans(skill_num + 8, (int)(skill_time - now));
		al_draw_text(font_48, al_map_rgb(255, 255, 255), WINDOW_W - 550, 900, 0, skill_num);
	}
}

static void battle_on_key_down(int keycode) {
    if (keycode == ALLEGRO_KEY_TAB)
        draw_gizmos = !draw_gizmos;
    if (keycode == ALLEGRO_KEY_R && restart == false) {
    	restart = true;
    	restart_time = al_get_time();
    	al_stop_sample(&bgm_id);
	}
	if (keycode == ALLEGRO_KEY_BACKSPACE) {
		game_change_scene(scene_menu_create());
		al_rest(1.5);
	}
	if (keycode == ALLEGRO_KEY_E) {
		paused ^= true;
		if (paused == 0) {
			al_set_mouse_xy(game_display, plane.x, plane.y);
			mouse_x = plane.x;
			mouse_y = plane.y;
		}
	}
}

static void update(void) {
	if (mode == dSTORY) {
		story_update();
	}
	else if (mode == dBATTLE) {
		battle_update();
	}
}

static void draw_draw_plane(MovableObject obj) {
	if (obj.hidden)
        return;
    
    al_draw_bitmap(obj.img, round(obj.x - ww / 2), round(obj.y - hh / 2), 0);
    if (draw_gizmos) {
        al_draw_filled_rectangle(round(obj.x - obj.w / 2), round(obj.y - obj.h / 2),
            round(obj.x + obj.w / 2) + 1, round(obj.y + obj.h / 2) + 1, al_map_rgb(255, 0, 0));
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
	if (mode == dSTORY) {
		story_draw();
	}
	else if (mode == dBATTLE) {
		battle_draw();
	}
}

static void destroy(void) {
    al_destroy_bitmap(img_background);
    al_destroy_bitmap(img_plane);
    al_destroy_sample(bgm);
    al_destroy_bitmap(img_bullet);
    stop_bgm(bgm_id);
    stop_bgm(battle_id);
    stop_bgm(boss_id);
    stop_bgm(end_id);
    game_log("Start scene destroyed");
}

static void on_key_down(int keycode) {
	if (mode == dSTORY) {
		story_on_key_down(keycode);
	}
	else if (mode == dBATTLE) {
		battle_on_key_down(keycode);
	}
}

static void on_mouse_down(int btn, int x, int y, int dz) {
	if (mode == dBATTLE && btn == 2) {
		if (power > 0) {
			power -= 1;
			if (staff_name == 0) {
				skill_radius = 10;
			}
			if (staff_name == 1) {
				skill = true;
				skill_time = al_get_time() + 10;
			}
		}
	}
}
// TODO: Add more event callback functions such as keyboard, mouse, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_story_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Story";
    scene.initialize = &init;
    scene.update = &update;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;
    scene.on_mouse_down = &on_mouse_down;
    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Start scene created");
    return scene;
}
