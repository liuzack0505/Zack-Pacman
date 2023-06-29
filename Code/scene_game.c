#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"


// [HACKATHON 2-0]
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
int game_main_Score = 0;
ALLEGRO_TIMER* power_up_timer;
ALLEGRO_TIMER* speed_up_timer;
ALLEGRO_TIMER* invisible_timer;
ALLEGRO_TIMER* redirection_timer;
bool game_over = false;
bool game_victory = false;

/* Internal variables*/
static const int redirection_duration = 100;
static const int power_up_duration = 7;
static const int invisible_durtion = 4;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
static bool pow_up=false;
bool debug_mode = false;
bool cheat_mode = false;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);
static void render_init_screen(void);
static void draw_hitboxes(void);

static void init(void) {
	game_over = false;
	game_victory = false;
	game_main_Score = 0;
	// create map
	basic_map = create_map(NULL);
	// [TODO]
	// Create map from .txt file and design your own map !!
	//basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}

	// allocate ghost memory
	// [HACKATHON 2-1]
	// TODO: Allocate dynamic memory for ghosts array.
	/*
	ghosts = (...)malloc(sizeof(...) * GHOST_NUM)
	*/
	ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
	if (!ghosts) {
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// [HACKATHON 2-2]
		// TODO: create a ghost.
		// Try to look the definition of ghost_create and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {

			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);
			if (!ghosts[i])
				game_abort("error creating ghost\n");

		}
	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
	speed_up_timer = al_create_timer(1.0f);
	invisible_timer = al_create_timer(1.0f);
	redirection_timer = al_create_timer(1.0f);
	if (!power_up_timer)
		game_abort("Error on create timer\n");
	return;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->	objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// [HACKATHON 1-3]
	// TODO: check which item you are going to eat and use pacman_eatItem to deal with it.

	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		game_main_Score += 100;
		pacman_eatItem(pman, '.');
		basic_map->map[Grid_y][Grid_x] = ' ';
		basic_map->beansCount--;
		break;
	case 'P':
		game_main_Score += 100;
		pacman_eatItem(pman, 'P');
		pman->powerUp= true;
		game_log("power_up");
		for (int x = 0; x < GHOST_NUM; x++) {
			ghost_toggle_FLEE(ghosts[x], true);
		}
		al_set_timer_count(power_up_timer, 0);
		basic_map->map[Grid_y][Grid_x] = ' ';
		break;
	case 'S':
		game_main_Score += 100;
		pacman_eatItem(pman, 'S');
		pman->speedUp = true;
		game_log("speed up");
		basic_map->map[Grid_y][Grid_x] = ' ';
		break;
	case 'D':
		game_main_Score += 100;
		pacman_eatItem(pman, 'D');
		pman->invisible = true;
		game_log("ghost invisible");
		basic_map->map[Grid_y][Grid_x] = ' ';
		break;
	case 'R':
		game_main_Score += 100;
		pacman_eatItem(pman, 'R');
		pman->redirection = true;
		game_log("pacman redirection");
		basic_map->map[Grid_y][Grid_x] = ' ';
		break;
	default:
		break;
	}

	// [HACKATHON 1-4]
	// erase the item you eat from map
	// be careful no erasing the wall block.
	/*
		basic_map->map...;
	*/
}
static void status_update(void) {
	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN)
			continue;
		// [TODO]
		// use getDrawArea(..., GAME_TICK_CD) and RecAreaOverlap(..., GAME_TICK_CD) functions to detect
		// if pacman and ghosts collide with each other.
		// And perform corresponding operations.
		// [NOTE]
		// You should have getDrawAreasome branch here if you want to implement power bean mode.
		// Uncomment Following Code
		RecArea ghost_loc = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		RecArea pacman_loc = getDrawArea(pman->objData, GAME_TICK_CD);
		if ((pman->powerUp==true)&&RecAreaOverlap(ghost_loc, pacman_loc))
		{
			RecArea room;
			room.x = pman->objData.Coord.x-1;
			room.y = pman->objData.Coord.y;
			room.h = 1;
			room.w = 3;
			game_log("eat ghost\n");
			ghost_collided(ghosts[i]);
			game_main_Score += 100;
			printGhostStatus(ghosts[i]->status);
			if(RecAreaOverlap(ghost_loc,room)) {
				game_log("back to room\n");
				ghosts[i]->status = BLOCKED;
			}
		}
		else {
			if ((game_over == false) && (RecAreaOverlap(ghost_loc, pacman_loc)))
			{
				game_log("collide with ghost\n");
				al_rest(1.0);
				pacman_die();
				game_over = true;
			}
		}
	}
}

static void update(void) {
	if (pman->redirection) {
		al_start_timer(redirection_timer);
		if (al_get_timer_count(redirection_timer) == power_up_duration) {
			al_stop_timer(redirection_timer);
			al_set_timer_count(redirection_timer, 0);
			pman->redirection = false;
			game_log("redirection canael");
		}
	}
	if (pman->invisible) {
		al_start_timer(invisible_timer);
		if (al_get_timer_count(invisible_timer) == invisible_durtion) {
			al_stop_timer(invisible_timer);
			al_set_timer_count(invisible_timer, 0);
			pman->invisible = false;
			game_log("visible");
		}
	}
	if (pman->speedUp)
	{
		al_start_timer(speed_up_timer);
		pman->speed = 4;
		if (al_get_timer_count(speed_up_timer) == power_up_duration) {
			al_stop_timer(speed_up_timer);
			al_set_timer_count(speed_up_timer, 0);
			pman->speedUp = false;
			pman->speed = 2;
			game_log("back to normal");
		}
	}
	if (pman->powerUp==true) {
		al_start_timer(power_up_timer);
		if (al_get_timer_count(power_up_timer) == power_up_duration) {
			al_stop_timer(power_up_timer);
			al_set_timer_count(power_up_timer, 0);
			pman->powerUp = false;
			for (int x = 0; x < GHOST_NUM; x++) {
				ghost_toggle_FLEE(ghosts[x], false);
			}
			game_log("back to normal");
		}
		else {
			for (int x = 0; x < GHOST_NUM; x++) {
				if (ghosts[x]->status == FLEE) {
					ghost_move_script_FLEE(ghosts[x], basic_map, pman);
				}
				else {
					ghost_move_script_GO_IN(ghosts[x], basic_map);
				}
			}
		}
	}

	if (game_over) {
		al_start_timer(pman->death_anim_counter);
		/*
			[TODO]
			start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
			game_change_scene(...);
		*/
		if (al_get_timer_count(pman->death_anim_counter) > 165) {
			al_stop_timer(pman->death_anim_counter);
			return;
		}
		//game_change_scene(fail_picture_create());
	}
	else if (game_victory) {
		return;
	}
	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	if (basic_map->beansCount == 0) {
			game_victory = true;
	}
	for (int i = 0; i < GHOST_NUM; i++)
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {
	char score_board[50];

	if (game_over && al_get_timer_count(pman->death_anim_counter) > 165) {
		sprintf_s(score_board, 50 * sizeof(char), "Score:%d", game_main_Score);
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_scaled_bitmap(
			pman->game_over,
			0, 0,
			al_get_bitmap_width(pman->game_over), al_get_bitmap_height(pman->game_over),
			110, 300,
			al_get_bitmap_width(pman->game_over)*2, al_get_bitmap_height(pman->game_over)*2,
			0
		);
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2, 550,
			ALLEGRO_ALIGN_CENTER,
			"PLEASE PRESS Z TO TRY AGAIN"
		);
	}
	else if (game_victory) {
		game_log("print game VICTORY ");
		sprintf_s(score_board, 50 * sizeof(char), "Score:%d", game_main_Score);
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(
			menuFont_big,
			al_map_rgb(178, 34, 34),
			SCREEN_W / 2, 350,
			ALLEGRO_ALIGN_CENTER,
			"WIN"
		);
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2, 500,
			ALLEGRO_ALIGN_CENTER,
			score_board
		);
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2, 700,
			ALLEGRO_ALIGN_CENTER,
			"PLEASE PRESS Z TO GET HIGHER SCORES"
		);
	}
	//	[TODO]
	//	Draw scoreboard, something your may need is sprinf();
	else {
		sprintf_s(score_board, 50 * sizeof(char), "Score:%d", game_main_Score);
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 - 15, 25,
			ALLEGRO_ALIGN_CENTER,
			score_board
		);
		al_draw_text(
			menuFont_small ,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2, 725,
			ALLEGRO_ALIGN_CENTER,
			"PRESS SPACE TO CHANGE COLOR"
		);

		draw_map(basic_map);

		pacman_draw(pman);
		if (game_over)
			return;
		// no drawing below when game over
		for (int i = 0; i < GHOST_NUM; i++)
			if (!pman->invisible||(pman->invisible&&ghosts[i]->status==GO_IN)) {
				ghost_draw(ghosts[i]);
			}

		//debugging mode
		if (debug_mode) {
			draw_hitboxes();
		}

	}
}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
	al_destroy_timer(speed_up_timer);
	al_destroy_timer(power_up_timer);
	al_destroy_timer(invisible_timer);
	al_destroy_timer(redirection_timer);
	pacman_destroy(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_destroy(ghosts[i]);
	}
	delete_map(basic_map);
}

static void on_key_down(int key_code) {
	switch (key_code)
	{
		// [HACKATHON 1-1]	
		// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function pacman_NextMove to set the pacman's next move direction.

	case ALLEGRO_KEY_W:
		if (pman->redirection) {
			pacman_NextMove(pman, DOWN);
		}
		else {
			pacman_NextMove(pman, UP);
		}
		break;
	case ALLEGRO_KEY_A:
		if (pman->redirection) {
			pacman_NextMove(pman, RIGHT);
		}
		else {
			pacman_NextMove(pman, LEFT);
		}
		break;
	case ALLEGRO_KEY_S:
		if (pman->redirection) {
			pacman_NextMove(pman, UP);
		}
		else {
			pacman_NextMove(pman, DOWN);
		}
		break;
	case ALLEGRO_KEY_D:
		if (pman->redirection) {
			pacman_NextMove(pman, LEFT);
		}
		else {
			pacman_NextMove(pman, RIGHT);
		}
		break;
	case ALLEGRO_KEY_C:
		cheat_mode = !cheat_mode;
		if (cheat_mode)
			printf("cheat mode on\n");
		else
			printf("cheat mode off\n");
		break;
	case ALLEGRO_KEY_G:
		debug_mode = !debug_mode;
		break;
	case ALLEGRO_KEY_Z:
		if (game_over) {
			game_change_scene(scene_menu_create(1));
		}
		else if (game_victory) {
			game_change_scene(scene_menu_create(1));
		}
		break;
	case ALLEGRO_KEY_SPACE:
		basic_map->map_r = generateRandomNumber(0, 255);
		basic_map->map_g = generateRandomNumber(0, 255);
		basic_map->mag_b = generateRandomNumber(0, 255);
	default:
		break;
	}

}

static void on_mouse_down(void) {
	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
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