#ifndef FONT_H
#define FONT_H

#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glfw.h>

struct font {
	GLuint tex;
	int char_width, char_height;
	int buffer_width, buffer_height;
};

struct font* font_load(char*);
void font_delete(struct font*);

void font_draw(struct font*, char *string);

#endif /* FONT_H */
