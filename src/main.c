#include <GL/glfw.h>

#include "window.h"
#include "key.h"

void init() {
	glClearColor(0.1, 0.1, 0.1, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void update() {
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

	glfwCloseWindow();

	glfwTerminate();

	return 0;
}
