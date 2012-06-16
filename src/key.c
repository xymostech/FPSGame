#include "key.h"

/* gets an array of keys */
int *get_curr_keys() {
	static int curr[GLFW_KEY_LAST+1];
	return curr;
}

/* gets an array of keys pressed last loop */
int *get_last_keys() {
	static int last[GLFW_KEY_LAST+1];
	return last;
}

/*
 * set_key
 *
 * GLFW callback for keypresses. On keypress,
 * set the state of get_curr_keys()
 */
void GLFWCALL set_key(int key, int state) {
	get_curr_keys()[key] = (state == GLFW_PRESS);
}

/*
 * update_keys
 *
 * callback for each loop, which copies information
 * into get_last_keys so we can observe differences
 */
void update_keys() {
	memcpy(get_last_keys(), get_curr_keys(), GLFW_KEY_LAST+1);
}

/* checks whether a key is currently pressed */
int key_pressed(int key) {
	return get_curr_keys()[key];
}

/* checks whether a key was pressed between the last frame and now */
int key_just_pressed(int key) {
	return get_curr_keys()[key] && !get_last_keys()[key];
}
