#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"

/* Internal Variables*/
static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_BITMAP* hideTitle = NULL;
static ALLEGRO_SAMPLE_ID menuBGM;
static ALLEGRO_SAMPLE_ID hidesceneBGM;
static int gameTitleW ;
static int gameTitleH ;
static int hideTitleW ;
static int hideTitleH ;
bool change;

// [HACKATHON 3]
// TARGET : use a clickable button to enter setting scene.
// For `Button` struct(object) is defined in `scene_menu_object.h` and `scene_menu_object.c`
// STRONGLY recommend you trace both of them first. 

//	[HACKATHON 3-1]
//	TODO: Declare variable for button
//	Uncomment and fill the code below
static Button btnSettings;

static void init() {

	// [HACKATHON 3-2]
	// TODO: Create button to settings
	//	Uncomment and fill the code below
	game_log("menu init");
	change = false;
	btnSettings = button_create(730, 20, 50, 50, "Assets/settings.png", "Assets/settings2.png");
	hide_condition = 0;
	gameTitle = load_bitmap("Assets/title.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);
	stop_bgm(menuBGM);
	menuBGM = play_bgm(themeMusic, music_volume);
	game_log("music volume=%0.6f",music_volume);
}
static void to_hide_init() {

	// [HACKATHON 3-2]
	// TODO: Create button to settings
	//	Uncomment and fill the code below
	game_log("to hide init");
	btnSettings = button_create(730, 20, 50, 50, "Assets/settings.png", "Assets/settings2.png");
	gameTitle = load_bitmap("Assets/title.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);
}

static void draw() {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;

	//draw title image
	al_draw_scaled_bitmap(
		gameTitle,
		0, 0,
		gameTitleW, gameTitleH,
		offset_w, offset_h,
		gameTitleW * scale, gameTitleH * scale,
		0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ENTER\""
	);
	if (hide_condition == 1) {
		al_draw_text(
			menuFont_big,
			al_map_rgb(255, 48, 48),
			250, 100,
			ALLEGRO_ALIGN_CENTER,
			"Z"
		);
	}
	else if (hide_condition == 2) {
		al_draw_text(
			menuFont_big,
			al_map_rgb(255, 48, 48),
			250, 100,
			ALLEGRO_ALIGN_CENTER,
			"Z"
		);
		al_draw_text(
			menuFont_big,
			al_map_rgb(255, 48, 48),
			350, 100,
			ALLEGRO_ALIGN_CENTER,
			"A"
		);
	}
	else if (hide_condition == 3) {
		al_draw_text(
			menuFont_big,
			al_map_rgb(255, 48, 48),
			250, 100,
			ALLEGRO_ALIGN_CENTER,
			"Z"
		);
		al_draw_text(
			menuFont_big,
			al_map_rgb(255, 48, 48),
			350, 100,
			ALLEGRO_ALIGN_CENTER,
			"A"
		);
		al_draw_text(
			menuFont_big,
			al_map_rgb(255, 48, 48),
			450, 100,
			ALLEGRO_ALIGN_CENTER,
			"C"
		);
	}
	else if (hide_condition == 4) {
		al_draw_text(
			menuFont_big,
			al_map_rgb(255, 48, 48),
			250, 100,
			ALLEGRO_ALIGN_CENTER,
			"Z"
		);
		al_draw_text(
			menuFont_big,
			al_map_rgb(255, 48, 48),
			350, 100,
			ALLEGRO_ALIGN_CENTER,
			"A"
		);
		al_draw_text(
			menuFont_big,
			al_map_rgb(255, 48, 48),
			450, 100,
			ALLEGRO_ALIGN_CENTER,
			"C");
		al_draw_text(
			menuFont_big,
			al_map_rgb(255, 48, 48),
			550, 100,
			ALLEGRO_ALIGN_CENTER,
			"K");
	}

		// [HACKATHON 3-3]
		// TODO: Draw button
		// Uncomment and fill the code below
		 drawButton(btnSettings);
		
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	//	[HACKATHON 3-7]
	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
	btnSettings.hovered = pnt_in_rect(mouse_x, mouse_y, btnSettings.body);
}


//	[HACKATHON 3-8]
//	TODO: When btnSettings clicked, switch to settings scene
//	Uncomment and fill the code below

static void on_mouse_down() {
	if (btnSettings.hovered == true) {
		stop_bgm(menuBGM);
		game_change_scene(scene_settings_create());
	}
}

static void destroy() {
	al_destroy_bitmap(gameTitle);
	//	[HACKATHON 3-10]
	//	TODO: Destroy Button images
	//	Uncomment and fill the code below
	al_destroy_bitmap(btnSettings.default_img);
	al_destroy_bitmap(btnSettings.hovered_img);
}
static void on_key_down(int keycode) {

	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			stop_bgm(menuBGM);
			if (hide_condition == 4) {
				game_change_scene(scene_menu_create(2));
			}
			else {
				game_change_scene(scene_main_create());
			}
			break;
		case ALLEGRO_KEY_Z:
			if (hide_condition == 0) {
				hide_condition++;
				game_change_scene(scene_menu_create(3));
			}
			break;
		case ALLEGRO_KEY_A:
			if (hide_condition == 1) {
				hide_condition++;
				game_change_scene(scene_menu_create(3));
			}
			break;
		case ALLEGRO_KEY_C:
			if (hide_condition == 2) {
				hide_condition++;
				game_change_scene(scene_menu_create(3));
			}
			break;
		case ALLEGRO_KEY_K:
			if (hide_condition == 3) {
				hide_condition++;
				game_change_scene(scene_menu_create(3));
			}
			break;
		default:
			break;
	}
}
static void hide_init() {

	game_log("hide scene init");
	hideTitle = load_bitmap("Assets/kaipi_scene.png");
	hideTitleW = al_get_bitmap_width(hideTitle);
	hideTitleH = al_get_bitmap_height(hideTitle);
	stop_bgm(hidesceneBGM);
	hidesceneBGM = play_bgm(HIDE_SCENE_SOUND_EFFECT, music_volume);
}

static void hide_draw() {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_scaled_bitmap(
		hideTitle,
		0, 0,
		hideTitleW, hideTitleH,
		150, 75,
		hideTitleW*0.75, hideTitleH*0.75,
		0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(72, 118, 255),
		400,
		450,
		ALLEGRO_ALIGN_CENTER,
		"THIS ROOM IS SNORLAX ' HOME"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255,48,48),
		400,
		535,
		ALLEGRO_ALIGN_CENTER,
		"HE WANTS SOME CREATIVE POINTS"
	);
	al_draw_text(
		menuFont_small,
		al_map_rgb(255,127,36),
		400,
		625,
		ALLEGRO_ALIGN_CENTER,
		"DONT CLICK \"G\" \"C\" WHEN YOU ARE PLAYING GAME"
	);
	al_draw_text(
		menuFont_small,
		al_map_rgb(25, 154, 25),
		400,
		700,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ESC\" TO LEAVE"
	);

}

static void hide_destroy() {
	stop_bgm(hidesceneBGM);
	al_destroy_bitmap(hideTitle);
}
static void on_hide_key_down(int keycode) {

	switch (keycode) {
	case ALLEGRO_KEY_ESCAPE:
		game_change_scene(scene_menu_create(1));
		break;
	default:
		break;
	}
}

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.

Scene scene_menu_create(int a) {

	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	if (a == 1) {
		scene.name = "Menu";
		scene.initialize = &init;
		scene.draw = &draw;
		scene.destroy = &destroy;
		scene.on_key_down = &on_key_down;
		scene.on_mouse_move = &on_mouse_move;
		// [HACKATHON 3-9]
		// TODO: Register on_mouse_down.
		// Uncomment the code below.
		scene.on_mouse_down = &on_mouse_down;
		// TODO: Register more event callback functions such as keyboard, mouse, ...
		game_log("Menu scene created");
		return scene;
	}
	else if (a == 2) {
		scene.name = "hide_scene";
		scene.initialize = &hide_init;
		scene.draw = &hide_draw;
		scene.destroy = &hide_destroy;
		scene.on_key_down = &on_hide_key_down;
		game_log("hide scene created");
		return scene;
	}
	else if (a == 3) {
		scene.name = "to hide scene";
		scene.initialize = &to_hide_init;
		scene.draw = &draw;
		scene.destroy = &destroy;
		scene.on_key_down = &on_key_down;
		scene.on_mouse_move = &on_mouse_move;
		// [HACKATHON 3-9]
		// TODO: Register on_mouse_down.
		// Uncomment the code below.
		scene.on_mouse_down = &on_mouse_down;
		// TODO: Register more event callback functions such as keyboard, mouse, ...
		game_log("Menu scene created");
		return scene;
	}
}