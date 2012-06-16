/*
 * font.h
 *
 * Functions for loading fonts and drawing
 * them as textures by individual character
 *
 * Currently, the FreeType library is used to load
 * the fonts and read out their data. The library
 * itself is not contained within this project, so
 * it has to be compiled separately. However, this
 * will probably be added later on, to ease
 * cross-platform problems
 */

#ifndef FONT_H
#define FONT_H

#include <stdlib.h>

/* include freetype */
#include <ft2build.h>
#include FT_FREETYPE_H

/* include OpenGL for drawing */
#include <GL/glfw.h>

/*
 * font_char
 *
 * stores information about a single
 * character, to make correct drawing possible
 */
struct font_char {
	int width, height;
	int bearing_x, bearing_y;
	int advance;
};

/*
 * font
 *
 * stores an OpenGL texture with all
 * the necessary font data, as well as
 * information about characters and
 * the texture itself
 */
struct font {
	GLuint tex;
	int char_width, char_height;
	int buffer_width, buffer_height;
	struct font_char *char_data;
};

/* load and delete a font */
struct font* font_load(char*);
void font_delete(struct font*);

/*
 * font_draw
 *
 * Function for drawing a string of characters given
 * a font. This will do a bunch of translates and things
 * to get the fonts to line up right.
 *
 * There is probably a better and more portable way
 * (for using the fonts in things other than just writing
 * to the HUD) which I will come up with later
 */
void font_draw(struct font*, char*, int);

#endif /* FONT_H */
