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
	
	update_window();
}

void update_window() {
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, *window_width(), *window_height());
		gluPerspective(45, *window_width()/(float)*window_height(), 0.01, 1000);
	glMatrixMode(GL_MODELVIEW);
}
