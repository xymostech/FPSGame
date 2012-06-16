/*
 * model.h
 *
 * Functions for loading, drawing, and
 * testing hittest for models
 */
#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "polygon.h"
#include "vector.h"
#include "texture.h"

/* model struct */
struct model {
	/* filename that the model was loaded from */
	char *file;

	/* texture used for the model */
	struct texture *texture;

	/* linked list of polygons */
	struct polygon *polygons;
};

/* load and delete models */
struct model* model_load(char*);
void model_delete(struct model*);

/* draw a model */
void model_draw(struct model*);

/* hittest a model */
float model_hittest(struct model*, struct vector, struct vector);

#endif /* MODEL_H */
