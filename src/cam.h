/*
 * cam.h
 *
 * functions for setting a virtual camera by setting the perspective
 */

#ifndef CAM_H
#define CAM_H

#include <GL/glfw.h>
#include <math.h>

/* cam structure */
struct cam {
	/* cam position */
	float x, y, z;
	/* cam angles */
	float xang, yang;
};

/* setup the camera */
void setup_cam(struct cam*);

#endif /* CAM_H */
