#ifndef HUD_H
#define HUD_H

#include "window.h"
#include "font.h"

struct hud {
	struct font *my_font;
};

struct hud* hud_init();
void hud_delete(struct hud*);

void hud_draw(struct hud*);

#endif /* HUD_H */
