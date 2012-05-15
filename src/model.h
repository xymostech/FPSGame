#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "polygon.h"

struct model {
	char *file;

	struct polygon *polygons;
};

struct model* model_load(char*);
void model_delete(struct model*);

void model_draw(struct model*);

#endif /* MODEL_H */
