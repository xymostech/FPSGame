#include <GL/glfw.h>

#include "window.h"
#include "key.h"
#include "cam.h"
#include "player.h"
#include "world.h"

struct player *my_player;
struct world *my_world;

void init() {
	glClearColor(0.1, 0.1, 0.1, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	my_player = player_init();
	my_world = world_init();
}

void terminate() {
	player_delete(my_player);
	world_delete(my_world);
}

void pyramid() {
	glColor3f(1, 1, 1);

	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(0, 1, 0);
		glVertex3f(0, -0.5, 0.5);
		glVertex3f(0.5, -0.5, -0.5);
		glVertex3f(-0.5, -0.5, -0.5);
		glVertex3f(0, -0.5, 0.5);
		glVertex3f(0, 1, 0);
	glEnd();

	glColor3f(0, 0, 0);

	glBegin(GL_LINE_LOOP);
		glVertex3f(0, -0.5, 0.5);
		glVertex3f(0.5, -0.5, -0.5);
		glVertex3f(-0.5, -0.5, -0.5);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(0, 1, 0);
		glVertex3f(0, -0.5, 0.5);
		glVertex3f(0, 1, 0);
		glVertex3f(0.5, -0.5, -0.5);
		glVertex3f(0, 1, 0);
		glVertex3f(-0.5, -0.5, -0.5);
	glEnd();
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		glTranslatef(2, 0, -5);
		pyramid();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-2, 0, -5);
		pyramid();
	glPopMatrix();

	world_draw(my_world);
}

void update() {
	player_selfupdate(my_player);
	player_update(my_player, my_world);
}

int main(int argc, char const **argv) {
	glfwInit();

	glfwOpenWindow(800, 600, 0, 0, 0, 0, 16, 0, GLFW_WINDOW);

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
