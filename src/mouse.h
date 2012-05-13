#ifndef MOUSE_H
#define MOUSE_H

#include <GL/glfw.h>

#include "window.h"

int mouse_left_pressed();
int mouse_right_pressed();

int mouse_window_x();
int mouse_window_y();

void mouse_get_mouselook_diff(int *x, int *y);

#endif /* MOUSE_H */
