#include <GL/glfw.h>

#include "window.h"
#include "key.h"
#include "cam.h"
#include "player.h"
#include "world.h"
#include "hud.h"

struct player *my_player;
struct world *my_world;
struct hud *my_hud;

void init() {
	glClearColor(0.1, 0.1, 0.1, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwDisable(GLFW_MOUSE_CURSOR);

	my_player = player_init();
	my_world = world_init();
	my_hud = hud_init();
}

void terminate() {
	player_delete(my_player);
	world_delete(my_world);
	hud_delete(my_hud);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	world_draw(my_world);
	hud_draw(my_hud);
}

void update() {
	player_selfupdate(my_player, my_world);
	player_update(my_player, my_world);
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
