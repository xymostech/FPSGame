/*
 * game.c
 *
 * Main entry point for FPS program
 */

#include <GL/glfw.h>

#include "window.h"
#include "key.h"
#include "cam.h"
#include "player.h"
#include "world.h"
#include "hud.h"
#include "server.h"

/* pointer to user-controlled player */
struct player *my_player;
/* pointer to game world */
struct world *my_world;
/* pointer to game HUD */
struct hud *my_hud;
/* pointer to game server */
struct server *my_server;

/*
 * init(void)
 *
 * called one right after OpenGL finishes setting up
 *
 * should perform setup of global variables, and OpenGL routines
 */
void init() {
	/* setup OpenGL */
	glClearColor(0.1, 0.1, 0.1, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwDisable(GLFW_MOUSE_CURSOR);

	/* setup global vars */
	my_player = player_init(-1);
	my_world = world_init();
	my_hud = hud_init();
	my_server = server_init();

	/* connect to the server */
	server_connect(my_server);
}

/*
 * terminate(void)
 *
 * called once right before terminating glfw
 *
 * should free and clean up all global variables
 */
void terminate() {
	/* free global vars */
	player_delete(my_player);
	world_delete(my_world);
	hud_delete(my_hud);
	
	/* disconnect from the server, then free */
	server_disconnect(my_server);
	server_delete(my_server);
}

/*
 * draw(void)
 *
 * called once every game loop after updating
 */
void draw() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	world_draw(my_world);
	hud_draw(my_hud);
}

/*
 * update(void)
 *
 * called once at the beginning of every game loop
 */
void update() {
	player_selfupdate(my_player, my_world, my_server);
	player_update(my_player, my_world);
	world_update(my_world, my_server);
	server_handle_updates(my_server, my_player, my_world);
}

/*
 * main
 *
 * entry point for FPS game
 */
int main(int argc, char const **argv) {
	/* initialize glfw */
	glfwInit();

	glfwOpenWindow(800, 600, 8, 8, 8, 8, 16, 0, GLFW_WINDOW);

	glfwSetWindowSizeCallback(set_window_size);
	glfwSetKeyCallback(set_key);

	init();

	int running = 1;

	/* main game loop */
	while(running) {
		update();
		update_keys();

		draw();

		glfwSwapBuffers();

		/* exit upon closing or pressing esc */
		running = !glfwGetKey(GLFW_KEY_ESC) &&
		          glfwGetWindowParam(GLFW_OPENED);
		
		/* sleep 1/100 sec, producing ~100 fps */
		glfwSleep(0.01);
	}

	terminate();

	/* terminate glfw */
	glfwCloseWindow();
	glfwTerminate();

	return 0;
}
