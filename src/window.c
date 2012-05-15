#include "window.h"

int *window_width() {
	static int w;
	return &w;
}

int *window_height() {
	static int h;
	return &h;
}

void GLFWCALL set_window_size(int w, int h) {
	*window_width() = w;
	*window_height() = h;
	
	window_set_perspective();
}

void window_set_perspective() {
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, *window_width(), *window_height());
		gluPerspective(45, *window_width()/(float)*window_height(), 0.01, 1000);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
}

void window_set_2d() {
	int half_width = *window_width() / 2;
	int half_height = *window_height() / 2;
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, *window_width(), *window_height());
		gluOrtho2D(-half_width, half_width, -half_height, half_height);
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_DEPTH_TEST);
}
