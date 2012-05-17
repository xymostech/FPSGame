#include "model.h"

void parse_vertex(char *str, int *vert, int *tvert, int *nvert) {
	char *token, *save;
	token = save = str;
	while (*save != '/' && *save) {
		++save;
	}
	if (!*save) {
		*vert = atoi(token);
		*tvert = 0;
		*nvert = 0;
		return;
	}
	*save = 0;
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

struct model* model_load(char *filename) {
	struct model *model = malloc(sizeof(*model));

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

	char *line = malloc(0);
	size_t size = 0;
	getline(&line, &size, file);
	while (!feof(file)) {
		char *token, *save, *rest;
		token = strtok_r(line, " \t\n", &save);
		if (token == NULL) {
			continue;
		}
		if (strcmp(token, "v") == 0) {
			rest = strtok_r(NULL, "\n", &save);
			float x, y, z;
			sscanf(rest, "%f %f %f", &x, &y, &z);
			struct point *point = point_init(x, y, z, 0, 0, 0, 0, 0);
			point->next = points;
			points = point;
			num_points++;
		} else if (strcmp(token, "vt") == 0) {
			rest = strtok_r(NULL, "\n", &save);
			float x, y;
			sscanf(rest, "%f %f", &x, &y);
			struct point *point = point_init(0, 0, 0, x, y, 0, 0, 0);
			point->next = tex_points;
			tex_points = point;
			num_tex_points++;
		} else if (strcmp(token, "vn") == 0) {
			rest = strtok_r(NULL, "\n", &save);
			float x, y, z;
			sscanf(rest, "%f %f %f", &x, &y, &z);
			struct point *point = point_init(0, 0, 0, 0, 0, x, y, z);
			point->next = norm_points;
			norm_points = point;
			num_norm_points++;
		} else if (strcmp(token, "f") == 0) {
			struct polygon *polygon = polygon_init();
			token = strtok_r(NULL, " \t\n", &save);
			int point_count = 0;
			while (token) {
				int v, tv, nv;
				parse_vertex(token, &v, &tv, &nv);
				point_count++;
				struct point zero_point;
				zero_point.x = 0;
				zero_point.y = 0;
				zero_point.z = 0;
				zero_point.tu = 0;
				zero_point.tv = 0;
				zero_point.nx = 0;
				zero_point.ny = 0;
				zero_point.nz = 0;
				struct point *ppoint = lookup_point(points, num_points, v);
				struct point *tpoint = lookup_point(tex_points, num_tex_points, tv);
				if (!tpoint) {
					tpoint = &zero_point;
				}
				struct point *npoint = lookup_point(norm_points, num_norm_points, nv);
				if (!npoint) {
					npoint = &zero_point;
				}
				struct point *point =
				      point_init(ppoint->x, ppoint->y, ppoint->z,
				                 tpoint->tu, tpoint->tv,
				                 npoint->nx, npoint->ny, npoint->nz);
				polygon_add_point(polygon, point);
				token = strtok_r(NULL, " \t\n", &save);
			}
			if (point_count == 3) {
				polygon_setup_hittest(polygon);
			}
			polygon->next = model->polygons;
			model->polygons = polygon;
		} else if (strcmp(token, "usetex") == 0) {
			rest = strtok_r(NULL, "\n", &save);
			model->texture = texture_load(rest);
		}
		getline(&line, &size, file);
	}
	free(line);

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

void model_draw(struct model *model) {
	if (model->texture) {
		glEnable(GL_TEXTURE_2D);
		texture_use(model->texture);
	}
	struct polygon *poly = model->polygons;
	while (poly) {
		polygon_draw(poly);
		poly = poly->next;
	}
	if (model->texture) {
		glDisable(GL_TEXTURE_2D);
	}
}

float model_hittest(struct model *model, struct vector start, struct vector dir) {
	struct polygon *poly = model->polygons;
	float best = -1, test;
	while (poly) {
		test = polygon_hittest(poly, start, dir);
		if (best < 0) {
			best = test;
		} else if (test > 0 && test < best) {
			best = test;
		}
		poly = poly->next;
	}
	return best;
}
