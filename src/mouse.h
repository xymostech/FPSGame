/*
 * mouse.h
 *
 * mouse functions to check the state of the mouse
 */
#ifndef MOUSE_H
#define MOUSE_H

#include <GL/glfw.h>

#include "window.h"

/* check if mouse buttons are pressed */
int mouse_left_pressed();
int mouse_right_pressed();

/* check the mouse position */
int mouse_window_x();
int mouse_window_y();

/* find how much the mouse has moved since the last check */
void mouse_get_mouselook_diff(int *x, int *y);

#endif /* MOUSE_H */
