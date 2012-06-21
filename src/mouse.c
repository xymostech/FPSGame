#include "mouse.h"

/* check the state of the left mouse button */
int mouse_left_pressed() {
	return glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

/* check the state of the right mouse button */
int mouse_right_pressed() {
	return glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}

/* check the mouse x position */
int mouse_window_x() {
	int x, y;
	glfwGetMousePos(&x, &y);
	return x - *window_width() / 2;
}

/* check the mouse y position */
int mouse_window_y() {
	int x, y;
	glfwGetMousePos(&x, &y);
	return *window_height() / 2 - y;
}

/* check how much the mouse has changed since the last call */
void mouse_get_mouselook_diff(int *x, int *y) {
	/* set the mouse position */
	*x = mouse_window_x();
	*y = mouse_window_y();
	/* set the mouse position to the middle */
	glfwSetMousePos(*window_width()/2, *window_height()/2);
}
