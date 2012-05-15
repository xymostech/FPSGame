#ifndef FONT_H
#define FONT_H

#include <stdlib.h>

struct font {
	
};

struct font* font_load(char*);
void font_delete(struct font*);

void font_draw(struct font*, char *string);

#endif /* FONT_H */
