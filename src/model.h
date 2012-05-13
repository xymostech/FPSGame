#ifndef MODEL_H
#define MODEL_H

#include "polygon.h"

struct model {
	char *name;

	struct polygon *polygons;
};

struct model* model_load(char*);
void model_delete(struct model*);

void model_draw(struct model*);

#endif /* MODEL_H */
