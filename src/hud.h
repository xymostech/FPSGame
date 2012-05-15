#ifndef HUD_H
#define HUD_H

#include "window.h"

struct hud {
	
};

struct hud* hud_init();
void hud_delete(struct hud*);

void hud_draw(struct hud*);

#endif /* HUD_H */
