#include "hud.h"

struct hud* hud_init() {
	struct hud *hud = malloc(sizeof(*hud));

	return hud;
}

void hud_delete(struct hud *hud) {
	free(hud);
}

void hud_draw(struct hud *hud) {
	window_set_2d();
	
	glLoadIdentity();

	glColor3f(0, 0, 1);

	glBegin(GL_LINE_LOOP);
		glVertex2f(0, 5);
		glVertex2f(5, 0);
		glVertex2f(0, -5);
		glVertex2f(-5, 0);
	glEnd();

	window_set_perspective();
}
