#ifndef WINDOW_H
#define WINDOW_H

#include <stdlib.h>
#include <GL/glfw.h>

#include <stdio.h>

int *window_width();
int *window_height();

void GLFWCALL set_window_size(int, int);

void window_set_perspective();
void window_set_2d();

#endif /* WINDOW_H */
