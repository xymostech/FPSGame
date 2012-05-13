#include "mouse.h"

int mouse_left_pressed() {
	return glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

int mouse_right_pressed() {
	return glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}

int mouse_window_x() {
	int x, y;
	glfwGetMousePos(&x, &y);
	return x - *window_width() / 2;
}

int mouse_window_y() {
	int x, y;
	glfwGetMousePos(&x, &y);
	return *window_height() / 2 - y;
}

void mouse_get_mouselook_diff(int *x, int *y) {
	*x = mouse_window_x();
	*y = mouse_window_y();
	glfwSetMousePos(*window_width()/2, *window_height()/2);
}
