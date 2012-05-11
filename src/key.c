#include "key.h"

int *get_curr_keys() {
	static int curr[GLFW_KEY_LAST+1];
	return curr;
}

int *get_last_keys() {
	static int last[GLFW_KEY_LAST+1];
	return last;
}

void GLFWCALL set_key(int key, int state) {
	get_curr_keys()[key] = (state == GLFW_PRESS);
}

void update_keys() {
	memcpy(get_last_keys(), get_curr_keys(), GLFW_KEY_LAST+1);
}

int key_pressed(int key) {
	return get_curr_keys()[key];
}

int key_just_pressed(int key) {
	return get_curr_keys()[key] && !get_last_keys()[key];
}
