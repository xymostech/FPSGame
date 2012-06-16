#include "model.h"

/*
 * parse_vertex
 *
 * parses out a vertex from an OBJ string:
 * takes a string of the form "V/T/N" and stores the
 * values of V, T, and N. If T or N doesn't exist,
 * 0 is stored in the value
 */
void parse_vertex(char *str, int *vert, int *tvert, int *nvert) {
	/* save iterates through the string,
	 * token stores the beginning of the last
	 * token character */
	char *token, *save;
	token = save = str;
	/* move save up to the next character */
	while (*save != '/' && *save) {
		++save;
	}
	/* if we hit the end, save V and set
	 * T and N to 0 */
	if (!*save) {
		*vert = atoi(token);
		*tvert = 0;
		*nvert = 0;
		return;
	}

	/* continue for the second and third tokens */
	*vert = atoi(token);
	token = save+1;
	while (*save != '/' && *save) {
		++save;
	}
	if (!*save) {
		*tvert = atoi(token);
		*nvert = 0;
		return;
	}
	*tvert = atoi(token);
	token = save+1;
	while (*save) {
		++save;
	}
	*nvert = atoi(token);
}

/* lookup a point by indes in the linked list of points */
struct point* lookup_point(struct point *points, int num_points, int point) {
	if (point == 0) {
		return NULL;
	}
	while (point < num_points) {
		points = points->next;
		point++;
	}
	return points;
}

/* load a model by filename */
struct model* model_load(char *filename) {
	struct model *model = malloc(sizeof(*model));

	/* set the polygon and textures to null */
	model->polygons = NULL;
	model->texture = NULL;

	FILE *file = fopen(filename, "r");

	if (!file) {
		printf("Error loading %s: %s\n", filename, strerror(errno));
	}

	struct point *points = NULL;
	int num_points = 0;
	struct point *tex_points = NULL;
	int num_tex_points = 0;
	struct point *norm_points = NULL;
	int num_norm_points = 0;

	/* the point to be used when
	 * a point is not found or given */
	struct point zero_point;
	zero_point.x = 0;
	zero_point.y = 0;
	zero_point.z = 0;
	zero_point.tu = 0;
	zero_point.tv = 0;
	zero_point.nx = 0;
	zero_point.ny = 0;
	zero_point.nz = 0;

	/* read in lines from the file, and parse them */
	/* line = malloc(0) allows getline to be used, without
	 * taking up any actual space */
	char *line = malloc(0);
	size_t size = 0;
	getline(&line, &size, file);
	while (!feof(file)) {
		char *token, *save, *rest;
		/* split the line by whitespace */
		token = strtok_r(line, " \t\n", &save);
		/* ignore empty lines */
		if (token == NULL) {
			continue;
		}
		if (strcmp(token, "v") == 0) {
			/* load a vertex */
			rest = strtok_r(NULL, "\n", &save);
			float x, y, z;
			sscanf(rest, "%f %f %f", &x, &y, &z);
			struct point *point = point_init(x, y, z, 0, 0, 0, 0, 0);
			point->next = points;
			points = point;
			num_points++;
		} else if (strcmp(token, "vt") == 0) {
			/* load a vertex texture */
			rest = strtok_r(NULL, "\n", &save);
			float x, y;
			sscanf(rest, "%f %f", &x, &y);
			struct point *point = point_init(0, 0, 0, x, y, 0, 0, 0);
			point->next = tex_points;
			tex_points = point;
			num_tex_points++;
		} else if (strcmp(token, "vn") == 0) {
			/* load a vertex normal */
			rest = strtok_r(NULL, "\n", &save);
			float x, y, z;
			sscanf(rest, "%f %f %f", &x, &y, &z);
			struct point *point = point_init(0, 0, 0, 0, 0, x, y, z);
			point->next = norm_points;
			norm_points = point;
			num_norm_points++;
		} else if (strcmp(token, "f") == 0) {
			/* load a face */
			/* create a polygon */
			struct polygon *polygon = polygon_init();

			/* get all the points in the polygon */
			token = strtok_r(NULL, " \t\n", &save);
			int point_count = 0;
			while (token) {
				int v, tv, nv;
				/* parse out the vertex */
				parse_vertex(token, &v, &tv, &nv);
				point_count++;
				/* lookup the point, texture, and normal */
				struct point *ppoint = lookup_point(points, num_points, v);
				struct point *tpoint = lookup_point(tex_points, num_tex_points, tv);
				struct point *npoint = lookup_point(norm_points, num_norm_points, nv);
				/* if the texture or normal aren't given,
				 * use the zero point instead */
				if (!tpoint) {
					tpoint = &zero_point;
				}
				if (!npoint) {
					npoint = &zero_point;
				}

				/* create the point */
				struct point *point =
				      point_init(ppoint->x, ppoint->y, ppoint->z,
				                 tpoint->tu, tpoint->tv,
				                 npoint->nx, npoint->ny, npoint->nz);
				polygon_add_point(polygon, point);
				token = strtok_r(NULL, " \t\n", &save);
			}

			/* if there are three points, then we can do
			 * triangular hittesting */
			if (point_count == 3) {
				polygon_setup_hittest(polygon);
			}
			polygon->next = model->polygons;
			model->polygons = polygon;
		} else if (strcmp(token, "usetex") == 0) {
			/* use a given texture */
			rest = strtok_r(NULL, "\n", &save);
			model->texture = texture_load(rest);
		}
		getline(&line, &size, file);
	}
	free(line);

	/* free the loaded points, because we made copies earlier */
	struct point *next;
	while (points) {
		next = points->next;
		point_delete(points);
		points = next;
	}
	while (tex_points) {
		next = tex_points->next;
		point_delete(tex_points);
		tex_points = next;
	}
	while (norm_points) {
		next = norm_points->next;
		point_delete(norm_points);
		norm_points = next;
	}

	fclose(file);

	return model;
}

/* free the polygons, texture, and model */
void model_delete(struct model *model) {
	struct polygon *poly = model->polygons, *next_poly;
	while (poly) {
		next_poly = poly->next;
		polygon_delete(poly);
		poly = next_poly;
	}
	texture_delete(model->texture);
	free(model);
}

/* draw the model */
void model_draw(struct model *model) {
	/* use the given texture */
	if (model->texture) {
		glEnable(GL_TEXTURE_2D);
		texture_use(model->texture);
	}
	/* go through the polygon list, and draw each of them */
	struct polygon *poly = model->polygons;
	while (poly) {
		polygon_draw(poly);
		poly = poly->next;
	}
	if (model->texture) {
		glDisable(GL_TEXTURE_2D);
	}
}

/*
 * model_hittest
 *
 * Test whether a ray hits the model by calling
 * hittest on all of the polygons within the model
 */
float model_hittest(struct model *model, struct vector start, struct vector dir) {
	/* cycle through each polygon */
	struct polygon *poly = model->polygons;
	float best = -1, test;
	while (poly) {
		/* check if the ray hits this polygon */
		test = polygon_hittest(poly, start, dir);
		/* if it does, and is closer, store that */
		if (best < 0) {
			best = test;
		} else if (test > 0 && test < best) {
			best = test;
		}
		poly = poly->next;
	}
	/* store the best polygon hit, which will be -1 if none were hit */
	return best;
}
