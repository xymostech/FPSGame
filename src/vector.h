#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

struct vector {
	float x, y, z;
};

struct vector vect(float, float, float);

struct vector vector_add(struct vector, struct vector);
struct vector vector_sub(struct vector, struct vector);
struct vector vector_mult(struct vector, float);
struct vector vector_div(struct vector, float);

float vector_len(struct vector);

float vector_dot(struct vector, struct vector);
struct vector vector_cross(struct vector, struct vector);

#endif /* VECTOR_H */
