#include "hud.h"

/* initialize the hud */
struct hud* hud_init() {
	struct hud *hud = malloc(sizeof(*hud));

	/* load a pre-defined font */
	hud->my_font = font_load("res/LiberationMono-Regular.ttf");

	return hud;
}

/* free up the hud and font */
void hud_delete(struct hud *hud) {
	font_delete(hud->my_font);
	free(hud);
}

/* draw the hud */
void hud_draw(struct hud *hud) {
	/* switch to 2D coordinates */
	window_set_2d();
	
	glLoadIdentity();

	glColor3f(0, 0, 1);

	/* draw a diamond crosshair */
	glBegin(GL_LINE_LOOP);
		glVertex2f(0, 5);
		glVertex2f(5, 0);
		glVertex2f(0, -5);
		glVertex2f(-5, 0);
	glEnd();

	/* switch back to perspective */
	window_set_perspective();
}
