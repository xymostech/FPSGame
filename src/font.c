#include "font.h"

struct font* font_load(char *filename) {
	struct font *font = malloc(sizeof(*font));

	return font;
}

void font_delete(struct font *font) {
	free(font);
}

void font_draw(struct font *font, char *string) {
	
}
