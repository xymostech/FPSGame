#ifndef FONT_H
#define FONT_H

#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H

struct font {
	
};

struct font* font_load(char*);
void font_delete(struct font*);

void font_draw(struct font*, char *string);

#endif /* FONT_H */
