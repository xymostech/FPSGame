#include "model.h"

void parse_vertex(char *str, int *vert) {
	char *token, *save;
	token = strtok_r(str, "/", &save);
	if (token == NULL) {
		*vert = 0;
	} else {
		*vert = atoi(token);
	}
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

	FILE *file = fopen(filename, "r");

	if (!file) {
		printf("Error loading %s: %s\n", filename, strerror(errno));
	}

	struct point *points = NULL;
	int num_points = 0;

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
			struct point *point = point_init(x, y, z);
			point->next = points;
			points = point;
			num_points++;
		} else if (strcmp(token, "f") == 0) {
			struct polygon *polygon = polygon_init();
			token = strtok_r(NULL, " \t\n", &save);
			int point_count = 0;
			while (token) {
				int v;
				parse_vertex(token, &v);
				point_count++;
				polygon_add_point(polygon,
				                  lookup_point(points,
				                  num_points, v));
				token = strtok_r(NULL, " \t\n", &save);
			}
			if (point_count == 3) {
				polygon_setup_hittest(polygon);
			}
			polygon->next = model->polygons;
			model->polygons = polygon;
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
	free(model);
}

void model_draw(struct model *model) {
	struct polygon *poly = model->polygons;
	while (poly) {
		polygon_draw(poly);
		poly = poly->next;
	}
}
