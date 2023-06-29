// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
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

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
/* Internal Variables*/
static ALLEGRO_SAMPLE_ID menuBGM;
static Button volumn_bigger;
static Button volumn_smaller;
static Button volumn_mute;
static Button sound_effect_bigger;
static Button sound_effect_smaller;
static Button sound_effect_mute;


static void init() {//start picture

	game_log("menu init");
	stop_bgm(menuBGM);
	menuBGM = play_audio(DEAD_SOUND_EFFECT, effect_volume);
	volumn_bigger = button_create(550, 175, 75, 75, "Assets/volumn_bigger_1.png", "Assets/volumn_bigger_2.png");
	volumn_smaller= button_create(350, 175, 75, 75, "Assets/volumn_smaller_1.png", "Assets/volumn_smaller_2.png");
	volumn_mute = button_create(150, 175, 75, 75, "Assets/volumn_mute_1.png", "Assets/volumn_mute_2.png");
	sound_effect_bigger = button_create(550, 400, 75, 75, "Assets/volumn_bigger_1.png", "Assets/volumn_bigger_2.png");
	sound_effect_smaller = button_create(350, 400, 75, 75, "Assets/volumn_smaller_1.png", "Assets/volumn_smaller_2.png");
	sound_effect_mute = button_create(150, 400, 75, 75, "Assets/volumn_mute_1.png", "Assets/volumn_mute_2.png");
}


static void draw(void ){
	char a[50],b[50];
	sprintf_s(a,50*sizeof(char),"SOUND_VOLUME=%0.1f",music_volume*10);
	sprintf_s(b, 50 * sizeof(char), "EFFECT_VOLUME=%0.1f", effect_volume * 10);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	drawButton(volumn_bigger);
	drawButton(volumn_smaller);
	drawButton(volumn_mute);
	drawButton(sound_effect_bigger);
	drawButton(sound_effect_mute);
	drawButton(sound_effect_smaller);
	al_draw_text(
		menuFont,
		al_map_rgb(25, 154, 25),
		SCREEN_W / 2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ESC\"TO BACK TO MENU"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(25, 154, 25),
		SCREEN_W / 2,
		SCREEN_H - 475,
		ALLEGRO_ALIGN_CENTER,
		a
	);
	al_draw_text(
		menuFont,
		al_map_rgb(25, 154, 25),
		SCREEN_W / 2,
		SCREEN_H - 250,
		ALLEGRO_ALIGN_CENTER,
		b
	);

}
static void destroy() {
	al_destroy_bitmap(volumn_bigger.hovered_img);
	al_destroy_bitmap(volumn_bigger.default_img);
	al_destroy_bitmap(volumn_smaller.hovered_img);
	al_destroy_bitmap(volumn_smaller.default_img);
	al_destroy_bitmap(volumn_mute.hovered_img);
	al_destroy_bitmap(volumn_mute.default_img);
	al_destroy_bitmap(sound_effect_bigger.hovered_img);
	al_destroy_bitmap(sound_effect_bigger.default_img);
	al_destroy_bitmap(sound_effect_smaller.hovered_img);
	al_destroy_bitmap(sound_effect_smaller.default_img);
	al_destroy_bitmap(sound_effect_mute.hovered_img);
	al_destroy_bitmap(sound_effect_mute.default_img);
}
static void on_key_down(int keycode) {

	switch (keycode) {
	case ALLEGRO_KEY_ESCAPE:
		game_change_scene(scene_menu_create(1));
		break;
	default:
		break;
	}
}
static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {//where is mouse
	sound_effect_bigger.hovered = pnt_in_rect(mouse_x, mouse_y, sound_effect_bigger.body);
	sound_effect_smaller.hovered = pnt_in_rect(mouse_x, mouse_y, sound_effect_smaller.body);
	sound_effect_mute.hovered = pnt_in_rect(mouse_x, mouse_y, sound_effect_mute.body);
	volumn_bigger.hovered = pnt_in_rect(mouse_x, mouse_y, volumn_bigger.body);
	volumn_smaller.hovered = pnt_in_rect(mouse_x, mouse_y, volumn_smaller.body);
	volumn_mute.hovered = pnt_in_rect(mouse_x, mouse_y, volumn_mute.body);
}
static void on_mouse_down() {//click the mouse
	if (volumn_bigger.hovered) {
		music_volume = music_volume+0.1;
		stop_bgm(menuBGM);
		menuBGM = play_audio(DEAD_SOUND_EFFECT, music_volume);
	}
	else if (volumn_smaller.hovered) {
		if (music_volume > 0) {
			music_volume = music_volume - 0.1;
			stop_bgm(menuBGM);
			menuBGM = play_audio(DEAD_SOUND_EFFECT, music_volume);
		}
	}
	else if (volumn_mute.hovered) {
		music_volume = 0;
	}
	if (sound_effect_bigger.hovered) {
		effect_volume += 0.1;
		stop_bgm(menuBGM);
		menuBGM = play_audio(DEAD_SOUND_EFFECT, effect_volume);
	}
	else if (sound_effect_smaller.hovered) {
		if (effect_volume > 0) {
			effect_volume -= 0.1;
			stop_bgm(menuBGM);
			menuBGM = play_audio(DEAD_SOUND_EFFECT, effect_volume);
		}
	}
	else if (sound_effect_mute.hovered) {
		effect_volume = 0;
	}
}
// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}