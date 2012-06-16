/*
 * hud.h
 *
 * Code to draw a HUD (heads up display) by
 * changing the display to draw in 2D, and then
 * drawing flat objects, before changing the
 * perspective back
 *
 * Currently not a lot is implemented, and everything
 * has to be hard-coded, but there are plans in the
 * future for much more open-ended HUD elements
 */
#ifndef HUD_H
#define HUD_H

#include "window.h"
#include "font.h"

/*
 * hud struct
 *
 * right now only contains a font
 */
struct hud {
	struct font *my_font;
};

/* create and destroy a hud */
struct hud* hud_init();
void hud_delete(struct hud*);

/* draw the hud to the screen */
void hud_draw(struct hud*);

#endif /* HUD_H */
