#ifndef FONT_H
#define FONT_H

#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glfw.h>

struct font_char {
	int width, height;
	int bearing_x, bearing_y;
	int advance;
};

struct font {
	GLuint tex;
	int char_width, char_height;
	int buffer_width, buffer_height;
	struct font_char *char_data;
};

struct font* font_load(char*);
void font_delete(struct font*);

void font_draw(struct font*, char*, int);

#endif /* FONT_H */
