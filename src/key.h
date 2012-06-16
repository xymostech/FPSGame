/*
 * key.h
 *
 * Functions for checking what keys are pressed,
 * and GLFW callbacks for storing key state
 */
#ifndef KEY_H
#define KEY_H

#include <stdlib.h>
#include <string.h>
#include <GL/glfw.h>

/* callbacks to set keys, and update each loop */
void GLFWCALL set_key(int, int);
void update_keys();

/* check whether a key is pressed, or was pressed last frame */
int key_pressed(int);
int key_just_pressed(int);

#endif /* KEY_H */
