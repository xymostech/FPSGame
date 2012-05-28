#ifndef SERVPLAYER_H
#define SERVPLAYER_H

#include <stdlib.h>

struct servplayer {
	float x, y, z;
	float yvel;
};

struct servplayer *servplayer_init();
void servplayer_delete(struct servplayer*);

#endif /* SERVPLAYER_H */
