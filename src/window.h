#ifndef WINDOW_H
#define WINDOW_H

#include <stdlib.h>
#include <GL/glfw.h>

#include <stdio.h>

int *window_width();
int *window_height();

void GLFWCALL set_window_size(int, int);

void update_window();

#endif /* WINDOW_H */
