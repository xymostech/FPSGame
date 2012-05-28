#include "servplayer.h"

struct servplayer* servplayer_init() {
	struct servplayer *servplayer = malloc(sizeof(*servplayer));

	return servplayer;
}

void servplayer_delete(struct servplayer *servplayer) {
	free(servplayer);
}
