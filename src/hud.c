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

	window_set_perspective();
}
