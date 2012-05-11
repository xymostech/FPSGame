#ifndef CAM_H
#define CAM_H

#include <GL/glfw.h>
#include <math.h>

struct cam {
	float x, y, z;
	float xang, yang;
};

void setup_cam(struct cam*);

#endif /* CAM_H */
