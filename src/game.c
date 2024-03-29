#include <GL/glfw.h>

#include "window.h"
#include "key.h"
#include "cam.h"
#include "player.h"
#include "world.h"
#include "hud.h"
#include "server.h"

struct player *my_player;
struct world *my_world;
struct hud *my_hud;
struct server *my_server;

void init() {
	glClearColor(0.1, 0.1, 0.1, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwDisable(GLFW_MOUSE_CURSOR);

	my_player = player_init(-1);
	my_world = world_init();
	my_hud = hud_init();
	my_server = server_init();

	server_connect(my_server);
}

void terminate() {
	player_delete(my_player);
	world_delete(my_world);
	hud_delete(my_hud);
	
	server_disconnect(my_server);
	server_delete(my_server);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	world_draw(my_world);
	hud_draw(my_hud);
}

void update() {
	player_selfupdate(my_player, my_world, my_server);
	player_update(my_player, my_world);
	world_update(my_world, my_server);
	server_handle_updates(my_server, my_player, my_world);
}

int main(int argc, char const **argv) {
	glfwInit();

	glfwOpenWindow(800, 600, 8, 8, 8, 8, 16, 0, GLFW_WINDOW);

	glfwSetWindowSizeCallback(set_window_size);
	glfwSetKeyCallback(set_key);

	init();

	int running = 1;

	while(running) {
		update();
		update_keys();

		draw();

		glfwSwapBuffers();

		running = !glfwGetKey(GLFW_KEY_ESC) &&
		          glfwGetWindowParam(GLFW_OPENED);
		
		glfwSleep(0.01);
	}

	terminate();

	glfwCloseWindow();
	glfwTerminate();

	return 0;
}
