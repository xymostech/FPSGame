#ifndef KEY_H
#define KEY_H

#include <stdlib.h>
#include <string.h>
#include <GL/glfw.h>

void GLFWCALL set_key(int, int);
void update_keys();

int key_pressed(int);
int key_just_pressed(int);

#endif /* KEY_H */
