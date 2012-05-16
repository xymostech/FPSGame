#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glfw.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

struct texture {
	GLuint tex;
};

struct texture* texture_load(char*);
void texture_delete(struct texture*);

void texture_use(struct texture*);

#endif /* TEXTURE_H */
