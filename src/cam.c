#include "cam.h"

/*
 * setup_cam(struct cam)
 *
 * takes a camera struct, and correctly sets up the view perspective
 */
void setup_cam(struct cam *my_cam) {
	glLoadIdentity();
	/* TODO: don't use gluLookAt, roll my own glRotates or glMultMatrix */
	gluLookAt(my_cam->x, my_cam->y, my_cam->z,
	          my_cam->x+cos(my_cam->xang)*sin(my_cam->yang),
	          my_cam->y+cos(my_cam->yang),
	          my_cam->z+sin(my_cam->xang)*sin(my_cam->yang),
	          0, 1, 0);
}
